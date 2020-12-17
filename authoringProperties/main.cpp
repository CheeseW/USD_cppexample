#include <iostream>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/sphere.h>
#include <pxr/base/vt/value.h>
#include <pxr/base/vt/array.h>

int main() {
    auto stage = pxr::UsdStage::Open("HelloWorld.usda");
    auto xform = stage->GetPrimAtPath(pxr::SdfPath("/hello"));
    auto sphere = stage->GetPrimAtPath(pxr::SdfPath("/hello/world"));

    for (const auto& prop : xform.GetPropertyNames())
        std::cout<<prop.GetString()<<" ";
    std::cout<<std::endl;

    for (const auto& prop : sphere.GetPropertyNames())
        std::cout<<prop.GetString()<<" ";
    std::cout<<std::endl;

    auto extentAttr = sphere.GetAttribute(pxr::TfToken("extent"));

     // anyway to get type when coding?
     pxr::VtArray<pxr::GfVec3f> value;
#if 0
    extentAttr.Get(&value);
#else
     pxr::VtValue vvalue;
     extentAttr.Get(&vvalue);
     std::cout<<vvalue.GetTypeName()<<std::endl;
     value = vvalue.Get<pxr::VtArray<pxr::GfVec3f>>();
#endif
     std::cout<<value<<std::endl;

     auto radiusAttr = sphere.GetAttribute(pxr::TfToken("radius"));
     radiusAttr.Set(2.);

     extentAttr.Set(value*2);

     auto sphereSchema = pxr::UsdGeomSphere(sphere);
     auto colorAttr = sphereSchema.GetDisplayColorAttr();
     colorAttr.Set(pxr::VtArray<pxr::GfVec3f>{{0,0,1}});

     std::string usdString;
     stage->GetRootLayer()->ExportToString(&usdString);
     std::cout<<usdString<<std::endl;
     stage->GetRootLayer()->Save();
     return 0;
}
