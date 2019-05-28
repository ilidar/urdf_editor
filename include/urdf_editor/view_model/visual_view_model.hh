//
// Created by Denys Kotelovych on 30.03.19.
//

#ifndef URDF_EDITOR_VISUAL_VIEW_MODEL_HH
#define URDF_EDITOR_VISUAL_VIEW_MODEL_HH

#include "urdf_editor/view_model/base_view_model.hh"
#include "urdf_editor/utils/time.hh"

namespace urdf_editor {
namespace view_model {

class VisualViewModel : public BaseViewModel<urdf::Visual, VisualViewModel> {
public:
    explicit VisualViewModel(const urdf::VisualSharedPtr &model)
            : BaseViewModel<urdf::Visual, VisualViewModel>(model),
              geometryVm_(std::make_shared<GeometryViewModel>(model->geometry)),
              materialVm_(std::make_shared<MaterialViewModel>(model->material)) {
        if (model_->name.empty())
            model_->name = "visual_" + std::to_string(utils::TimeNowMilliseconds());
    }

public:
    QString Name() const { return QString::fromStdString(model_->name); }

    void Name(const QString &name) { model_->name = name.toStdString(); }

    const urdf::Pose &Origin() const { return model_->origin; }

    void Origin(const urdf::Pose &origin) { model_->origin = origin; }

    const MaterialViewModelPtr &Material() const { return materialVm_; }

    const GeometryViewModelPtr &Geometry() const { return geometryVm_; }

public:
    void Sync() override {
        geometryVm_->Sync();
        model_->geometry = geometryVm_->Model();
        model_->material = materialVm_->Model();
    }

private:
    GeometryViewModelPtr geometryVm_;
    MaterialViewModelPtr materialVm_;
};

using VisualViewModelPtr = std::shared_ptr<VisualViewModel>;
using V_VisualViewModelPtr = std::vector<VisualViewModelPtr>;

}
}

#endif //URDF_EDITOR_VISUAL_VIEW_MODEL_HH
