//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_URDF_VIEW_MODEL_HH
#define URDF_EDITOR_URDF_VIEW_MODEL_HH

#include <urdf/model.h>

#include "urdf_editor/view_model/link_view_model.hh"
#include "urdf_editor/view_model/joint_view_model.hh"

namespace urdf_editor {
namespace view_model {

class URDFViewModel {
public:
    explicit URDFViewModel();

public:
    const urdf::ModelSharedPtr &URDF() const { return urdf_; };

    const LinkViewModelPtr &RootLinkViewModel() const { return rootLink_; }

    QStringList LinkNames() const {
        QStringList result;
        std::transform(urdf_->links_.begin(), urdf_->links_.end(), std::back_inserter(result),
                       [](const std::pair<std::string, urdf::LinkSharedPtr> &pair) {
                           return QString::fromStdString(pair.first);
                       });
        return result;
    }

public:
    void New();

    bool Load(const QString &filePath);

    bool Save(const QString &filePath);

    void AddLink(const LinkViewModelPtr &linkVm);

    void CloneLink(const LinkViewModelPtr &linkVm);

    void RemoveLink(const LinkViewModelPtr &linkVm);

    void UpdateLink(const LinkViewModelPtr &oldLinkVm, const LinkViewModelPtr &newLinkVm);

private:
    urdf::ModelSharedPtr urdf_;
    LinkViewModelPtr rootLink_;
    std::int32_t cloneCount_;
};

}
}

#endif //URDF_EDITOR_URDF_VIEW_MODEL_HH
