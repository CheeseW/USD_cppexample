#include <iostream>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/xformCommonAPI.h>
#include <pxr/usd/usdGeom/xformable.h>
#include <pxr/usd/usd/variantSets.h>
#include <pxr/usd/usd/editContext.h>
// #include <pxr/base/vt/value.h>
// #include <pxr/base/vt/array.h>

int main() {
    std::string usdString;

    auto stage = pxr::UsdStage::Open("HelloWorld.usda");
    auto colorAttr = pxr::UsdGeomGprim::Get(stage, pxr::SdfPath("/hello/world")).GetDisplayColorAttr();
    colorAttr.Clear();
    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    auto rootPrim = stage->GetPrimAtPath(pxr::SdfPath("/hello"));
    auto vset = rootPrim.GetVariantSets().AddVariantSet("shadingVariant");
    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    vset.AddVariant("red");
    vset.AddVariant("blue");
    vset.AddVariant("green");
    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    vset.SetVariantSelection("red");
    {
        pxr::UsdEditContext ctxt(vset.GetVariantEditContext());
        colorAttr.Set(pxr::VtArray<pxr::GfVec3f>{{1,0,0}});
    }
    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    vset.SetVariantSelection("blue");
    {
        pxr::UsdEditContext ctxt(vset.GetVariantEditContext());
        colorAttr.Set(pxr::VtArray<pxr::GfVec3f>{{0,0,1}});
    }

    vset.SetVariantSelection("green");
    {
        pxr::UsdEditContext ctxt(vset.GetVariantEditContext());
        colorAttr.Set(pxr::VtArray<pxr::GfVec3f>{{0,1,0}});
    }
    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    stage->GetRootLayer()->ExportToString(&usdString);
    std::cout<<usdString<<std::endl;

    stage->ExportToString(&usdString,false);
    std::cout<<usdString<<std::endl;

    stage->GetRootLayer()->Export("HelloWorldWithVariants.usda");
    return 0;
}
