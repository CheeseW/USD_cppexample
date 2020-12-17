#include <iostream>

#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/tokens.h>
// #include <pxr/usd/usdGeom/sphere.h>
#include <pxr/usd/usdGeom/xform.h>
// #include <pxr/usd/usdGeom/xformCommonAPI.h>
// #include <pxr/usd/usdGeom/xformable.h>
// #include <pxr/usd/usd/variantSets.h>
// #include <pxr/usd/usd/editContext.h>

// not sure what's the best way to pass these USD objects

pxr::UsdStageRefPtr MakeInitialStage(const std::string path) {
    auto stage = pxr::UsdStage::CreateNew(path);
    pxr::UsdGeomSetStageUpAxis(stage, pxr::UsdGeomTokens->z);
    stage->SetStartTimeCode(0);
    stage->SetEndTimeCode(192);
    return stage;
}

pxr::UsdGeomXform AddReferenceToGeometry(pxr::UsdStageRefPtr& stage, const std::string path) {
    auto geom = pxr::UsdGeomXform::Define(stage, pxr::SdfPath(path));
    geom.GetPrim().GetReferences().AddReference("./top.geom.usd");
    return geom;
}

void AddSpin(pxr::UsdGeomXform top) {
    auto spin = top.AddRotateZOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("spin"));
    spin.Set(0.f,0);
    spin.Set(1440.f,192);
}

void AddTilt(pxr::UsdGeomXform top) {
    auto tilt = top.AddRotateXOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("tilt"));
    tilt.Set(12.f);
}

void AddOffset(pxr::UsdGeomXform top) {
    top.AddTranslateOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("offset")).Set(pxr::GfVec3f{0.f,0.1f,0.f});
}

void AddPrecession(pxr::UsdGeomXform top) {
    auto precess = top.AddRotateZOp(pxr::UsdGeomXformOp::PrecisionFloat, pxr::TfToken("precess"));
    precess.Set(0.f, 0);
    precess.Set(360.f, 192);
}


void Step1() {
    auto stage = MakeInitialStage("Step1.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 1: Start and end time codes");
    stage->Save();
}

void Step2() {
    auto stage = MakeInitialStage("Step2.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 2: Geometry reference");
    auto top = AddReferenceToGeometry(stage, "/Top");
    stage->Save();
}

void Step3() {
    auto stage = MakeInitialStage("Step3.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 3: Adding spin animation");
    auto top = AddReferenceToGeometry(stage, "/Top");
    AddSpin(top);
    stage->Save();
}

void Step4() {
    auto stage = MakeInitialStage("Step4.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 4: Adding tilt");
    auto top = AddReferenceToGeometry(stage, "/Top");
    AddTilt(top);
    AddSpin(top);
    stage->Save();
}

void Step4A() {
    auto stage = MakeInitialStage("Step4A.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 4: Adding spin, then tilt");
    auto top = AddReferenceToGeometry(stage, "/Top");
    AddSpin(top);
    AddTilt(top);
    stage->Save();
}

void Step5() {
    auto stage = MakeInitialStage("Step5.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 5: Adding precession and offset");
    auto top = AddReferenceToGeometry(stage, "/Top");
    AddPrecession(top);
    AddOffset(top);
    AddTilt(top);
    AddSpin(top);
    stage->Save();
}

void Step6() {
    std::string anim_layer_path = "./Step5.usda";

    auto stage = MakeInitialStage("Step6.usda");
    stage->SetMetadata(pxr::TfToken("comment"), "Step 6: Layer offset and animation");

    auto left = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Left"));
    auto left_top = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Left/Top"));
    left_top.GetPrim().GetReferences().AddReference(anim_layer_path, pxr::SdfPath("/Top"));

    auto middle = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Middle"));
    middle.AddTranslateOp().Set(pxr::GfVec3d{2., 0., 0.});
    auto middle_top = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Middle/Top"));
    middle_top.GetPrim().GetReferences().AddReference(anim_layer_path, pxr::SdfPath("/Top"), pxr::SdfLayerOffset(96));

    auto right = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Right"));
    right.AddTranslateOp().Set(pxr::GfVec3d{4., 0., 0.});
    auto right_top = pxr::UsdGeomXform::Define(stage, pxr::SdfPath("/Right/Top"));
    right_top.GetPrim().GetReferences().AddReference(anim_layer_path, pxr::SdfPath("/Top"), pxr::SdfLayerOffset(0, 0.25));

    stage->Save();
}

int main() {
    Step1();
    Step2();
    Step3();
    Step4();
    Step4A();
    Step5();
    Step6();
    return 0;
}
