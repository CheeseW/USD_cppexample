#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usd/prim.h>


int main() {
    auto stage = pxr::UsdStage::CreateNew("HelloWorldRedux.usda");
    auto xform = stage->DefinePrim(pxr::SdfPath("/hello"), pxr::TfToken("Xform"));
    auto sphere = stage->DefinePrim(pxr::SdfPath("/hello/world"), pxr::TfToken("Sphere"));
    stage->GetRootLayer()->Save();

    return 0;
}
