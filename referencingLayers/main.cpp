#include <iostream>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/usd/usdGeom/xformable.h>
// #include <pxr/base/vt/value.h>
// #include <pxr/base/vt/array.h>

int main() {
    std::string usdString;

    auto stage = pxr::UsdStage::Open("HelloWorld.usda");
    auto hello = stage->GetPrimAtPath(pxr::SdfPath("/hello"));
    stage->SetDefaultPrim(hello);
    pxr::UsdGeomXformCommonAPI(hello).SetTranslate(pxr::GfVec3d(4,5,6));

    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    stage->GetRootLayer()->Save();

    auto refStage = pxr::UsdStage::CreateNew("RefExample.usda");\
    auto refSphere = refStage->OverridePrim(pxr::SdfPath("/refSphere"));
    refStage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    refSphere.GetReferences().AddReference("./HelloWorld.usda");
    refStage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;
    refStage->GetRootLayer()->Save();

    auto refXform = pxr::UsdGeomXformable(refSphere);
    refXform.SetXformOpOrder(std::vector<pxr::UsdGeomXformOp>());
    refStage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    auto refSphere2 = refStage->OverridePrim(pxr::SdfPath("/refSphere2"));
    refSphere2.GetReferences().AddReference("./HelloWorld.usda");
    refStage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    refStage->GetRootLayer()->Save();

    auto overSphere = pxr::UsdGeomSphere::Get(refStage, pxr::SdfPath("/refSphere2/world"));
    overSphere.GetDisplayColorAttr().Set(pxr::VtArray<pxr::GfVec3f>{{1,0,0}});
    refStage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    refStage->GetRootLayer()->Save();

    refStage->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    return 0;
}
