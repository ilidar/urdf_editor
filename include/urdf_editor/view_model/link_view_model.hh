//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_LINK_VIEW_MODEL_HH
#define URDF_EDITOR_LINK_VIEW_MODEL_HH

#include "urdf_editor/utils/urdf_clone.hh"
#include "urdf_editor/view_model/geometry_view_model.hh"
#include "urdf_editor/view_model/material_view_model.hh"
#include "urdf_editor/view_model/collision_view_model.hh"
#include "urdf_editor/view_model/inertial_view_model.hh"
#include "urdf_editor/view_model/visual_view_model.hh"
#include "urdf_editor/view_model/joint_view_model.hh"

namespace urdf_editor {
namespace view_model {

class LinkViewModel;

using LinkViewModelPtr = std::shared_ptr<LinkViewModel>;
using V_LinkViewModelPtr = std::vector<LinkViewModelPtr>;

class LinkViewModel : public BaseViewModel<urdf::Link, LinkViewModel> {
public:
    explicit LinkViewModel(const urdf::LinkSharedPtr &model)
            : BaseViewModel<urdf::Link, LinkViewModel>(model),
              inertial_(std::make_shared<InertialViewModel>(model_->inertial)),
              joint_(std::make_shared<JointViewModel>(model_->parent_joint)) {
        for (const auto &visual : model_->visual_array)
            visuals_.emplace_back(new VisualViewModel(visual));
        for (const auto &collision : model_->collision_array)
            collisions_.emplace_back(new CollisionViewModel(collision));
    }

public:
    QString Name() const { return QString::fromStdString(model_->name); }

    void Name(const QString &name) { model_->name = name.toStdString(); }

    const InertialViewModelPtr &Inertial() const { return inertial_; }

    const V_VisualViewModelPtr &Visuals() const { return visuals_; }

    const V_CollisionViewModelPtr &Collisions() const { return collisions_; }

    const JointViewModelPtr &Joint() const { return joint_; }

    void UsedLinkNames(const QStringList &usedLinkNames) { joint_->UsedLinkNames(usedLinkNames); }

    bool Valid() const {
        return !model_->name.empty() && !joint_->UsedLinkNames().contains(QString::fromStdString(model_->name));
    }

    V_LinkViewModelPtr Children() const {
        V_LinkViewModelPtr result;
        for (const auto &child : model_->child_links)
            result.emplace_back(new LinkViewModel(child));
        return result;
    }

public:
    void Sync() override {
        joint_->Sync();
        for (const auto &visual : visuals_)
            visual->Sync();
        for (const auto &collision : collisions_)
            collision->Sync();

        model_->parent_joint = joint_->Model();
        model_->inertial = inertial_->Model();
        model_->visual_array.clear();
        std::transform(visuals_.begin(), visuals_.end(), std::back_inserter(model_->visual_array),
                       [](const VisualViewModelPtr &vm) { return vm->Model(); });
        if (!model_->visual_array.empty())
            model_->visual = model_->visual_array.front();

        model_->collision_array.clear();
        std::transform(collisions_.begin(), collisions_.end(), std::back_inserter(model_->collision_array),
                       [](const CollisionViewModelPtr &vm) { return vm->Model(); });
        if (!model_->collision_array.empty())
            model_->collision = model_->collision_array.front();
    }

    void Add(const VisualViewModelPtr &visual) {
        visuals_.push_back(visual);

        Sync();
    }

    void Remove(const VisualViewModelPtr &visual) {
        visuals_.erase(std::remove(visuals_.begin(), visuals_.end(), visual), visuals_.end());

        Sync();
    }

    void Add(const CollisionViewModelPtr &collision) {
        collisions_.push_back(collision);

        Sync();
    }

    void Remove(const CollisionViewModelPtr &visual) {
        collisions_.erase(std::remove(collisions_.begin(), collisions_.end(), visual), collisions_.end());

        Sync();
    }

private:
    InertialViewModelPtr inertial_;
    JointViewModelPtr joint_;
    V_VisualViewModelPtr visuals_;
    V_CollisionViewModelPtr collisions_;
};

}
}


#endif //URDF_EDITOR_LINK_VIEW_MODEL_HH
