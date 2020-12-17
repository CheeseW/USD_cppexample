#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/sphere.h>

int main() {
    auto stage = pxr::UsdStage::CreateNew("HelloWorld.usda");
    auto xformPrim = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/hello"));
    auto spherePrim = pxr::UsdGeomSphere::Define(stage, pxr::SdfPath("/hello/world"));
    stage->GetRootLayer()->Save();

    return 0;
}
