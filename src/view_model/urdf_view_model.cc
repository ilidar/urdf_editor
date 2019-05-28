//
// Created by Denys Kotelovych on 26.03.19.
//

#include <urdf_parser/urdf_parser.h>
#include <queue>

#include "urdf_editor/view_model/urdf_view_model.hh"

namespace urdf_editor {
namespace view_model {

URDFViewModel::URDFViewModel()
        : urdf_(new urdf::Model()),
          cloneCount_(0) {}

void URDFViewModel::New() {
    urdf_.reset(new urdf::Model());
    rootLink_ = nullptr;
}

bool URDFViewModel::Load(const QString &filePath) {
    New();

    if (!urdf_->initFile(filePath.toStdString())) {
        ROS_ERROR_STREAM_NAMED("urdf_editor", "Failed to parse urdf file");
        urdf_ = nullptr;
        return false;
    }

    rootLink_.reset(new LinkViewModel(urdf_->root_link_));

    return true;
}

bool URDFViewModel::Save(const QString &filePath) {
    std::unique_ptr<TiXmlDocument> tiXmlDoc(urdf::exportURDF(*urdf_));
    return tiXmlDoc->SaveFile(filePath.toStdString());
}

void URDFViewModel::AddLink(const LinkViewModelPtr &linkVm) {
    const auto &newJoint = linkVm->Joint();
    const auto &newParentLinkIt = urdf_->links_.find(newJoint->ParentLinkName().toStdString());
    if (newParentLinkIt != urdf_->links_.end()) {
        const auto &newParentLink = newParentLinkIt->second;
        newParentLink->child_links.push_back(linkVm->Model());
        newParentLink->child_joints.push_back(linkVm->Joint()->Model());
    }

    urdf_->joints_[newJoint->Name().toStdString()] = newJoint->Model();
    urdf_->links_[linkVm->Name().toStdString()] = linkVm->Model();

    for (const auto &visual : linkVm->Visuals())
        urdf_->materials_[visual->Material()->Name().toStdString()] = visual->Material()->Model();

    if (!urdf_->root_link_) {
        urdf_->root_link_ = linkVm->Model();
        urdf_->joints_.erase(urdf_->root_link_->parent_joint->name);
        urdf_->root_link_->parent_joint = nullptr;
        urdf_->root_link_->inertial = nullptr;
        rootLink_.reset(new LinkViewModel(urdf_->root_link_));
    }
}

void URDFViewModel::CloneLink(const LinkViewModelPtr &linkVm) {
    const auto &clone = linkVm->Clone();
    const auto &suffix = "_clone_" + QString::number(++cloneCount_);
    clone->Name(clone->Name() + suffix);
    clone->Joint()->Name(clone->Joint()->Name() + suffix);
    AddLink(clone);
}

void URDFViewModel::RemoveLink(const LinkViewModelPtr &linkVm) {
    const auto &link = linkVm->Model();

    if (link != urdf_->root_link_) {
        const auto &parentLink = urdf_->links_[link->parent_joint->parent_link_name];

        auto &parentLinkChildLinks = parentLink->child_links;
        parentLinkChildLinks.erase(std::remove(
                parentLinkChildLinks.begin(), parentLinkChildLinks.end(), link), parentLinkChildLinks.end());

        auto &parentLinkChildJoints = parentLink->child_joints;
        parentLinkChildJoints.erase(std::remove(
                parentLinkChildJoints.begin(),
                parentLinkChildJoints.end(), link->parent_joint), parentLinkChildJoints.end());
    } else {
        rootLink_ = nullptr;
    }

    std::queue<urdf::LinkSharedPtr> Q;
    Q.push(link);

    while (!Q.empty()) {
        auto top = Q.front();
        Q.pop();

        urdf_->links_.erase(top->name);
        if (top->parent_joint)
            urdf_->joints_.erase(top->parent_joint->name);

        for (const auto &visual : top->visual_array)
            urdf_->materials_.erase(visual->material_name);

        for (const auto &child : top->child_links)
            Q.push(child);
    }
}

void URDFViewModel::UpdateLink(const LinkViewModelPtr &oldLinkVm, const LinkViewModelPtr &newLinkVm) {
    const auto &oldJoint = oldLinkVm->Joint();

    urdf_->links_.erase(oldLinkVm->Name().toStdString());
    urdf_->joints_.erase(oldJoint->Name().toStdString());
    for (const auto &visual : oldLinkVm->Visuals())
        urdf_->materials_.erase(visual->Material()->Name().toStdString());

    const auto &oldParentLinkIt = urdf_->links_.find(oldJoint->ParentLinkName().toStdString());
    if (oldParentLinkIt != urdf_->links_.end()) {
        const auto &oldParentLink = oldParentLinkIt->second;

        auto &childLinks = oldParentLink->child_links;
        const auto &it1 = std::remove_if(
                childLinks.begin(), childLinks.end(),
                [&](const urdf::LinkSharedPtr &link) {
                    return link->name == oldLinkVm->Name().toStdString();
                });
        childLinks.erase(it1, childLinks.end());

        auto &childJoints = oldParentLink->child_joints;
        const auto &it2 = std::remove_if(
                childJoints.begin(), childJoints.end(),
                [&](const urdf::JointSharedPtr &joint) {
                    return joint->name == oldJoint->Name().toStdString();
                });
        childJoints.erase(it2, childJoints.end());
    }

    const auto &newJoint = newLinkVm->Joint();
    const auto &newParentLinkIt = urdf_->links_.find(newJoint->ParentLinkName().toStdString());
    if (newParentLinkIt != urdf_->links_.end()) {
        const auto &newParentLink = newParentLinkIt->second;
        newParentLink->child_links.push_back(newLinkVm->Model());
        newParentLink->child_joints.push_back(newLinkVm->Joint()->Model());
    }

    urdf_->joints_[newJoint->Name().toStdString()] = newJoint->Model();
    urdf_->links_[newLinkVm->Name().toStdString()] = newLinkVm->Model();
    for (const auto &visual : newLinkVm->Visuals())
        urdf_->materials_[visual->Material()->Name().toStdString()] = visual->Material()->Model();


    if (oldLinkVm->Model() == urdf_->root_link_) {
        urdf_->root_link_ = newLinkVm->Model();
        rootLink_.reset(new LinkViewModel(urdf_->root_link_));
    }
}

}
}
