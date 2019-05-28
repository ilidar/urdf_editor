//
// Created by Denys Kotelovych on 30.03.19.
//

#ifndef URDF_EDITOR_COLLISION_VIEW_MODEL_HH
#define URDF_EDITOR_COLLISION_VIEW_MODEL_HH

#include <QString>

#include "urdf_editor/view_model/base_view_model.hh"
#include "urdf_editor/view_model/geometry_view_model.hh"

namespace urdf_editor {
namespace view_model {

class CollisionViewModel : public BaseViewModel<urdf::Collision, CollisionViewModel> {
public:
    explicit CollisionViewModel(const urdf::CollisionSharedPtr &model)
            : BaseViewModel<urdf::Collision, CollisionViewModel>(model),
              geometryVm_(std::make_shared<GeometryViewModel>(model_->geometry)) {
        if (model_->name.empty())
            model_->name = "collision_" + std::to_string(utils::TimeNowMilliseconds());
    }

public:
    const urdf::Pose &Origin() const { return model_->origin; }

    void Origin(const urdf::Pose &origin) { model_->origin = origin; }

    QString Name() const { return QString::fromStdString(model_->name); }

    void Name(const QString &name) { model_->name = name.toStdString(); }

    const GeometryViewModelPtr &Geometry() const { return geometryVm_; }

public:
    void Sync() override {
        geometryVm_->Sync();
        model_->geometry = geometryVm_->Model();
    }

private:
    GeometryViewModelPtr geometryVm_;
};

using CollisionViewModelPtr = std::shared_ptr<CollisionViewModel>;
using V_CollisionViewModelPtr = std::vector<CollisionViewModelPtr>;

}
}

#endif //URDF_EDITOR_COLLISION_VIEW_MODEL_HH
