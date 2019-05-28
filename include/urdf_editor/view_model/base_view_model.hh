//
// Created by Denys Kotelovych on 30.03.19.
//

#ifndef URDF_EDITOR_BASE_VIEW_MODEL_HH
#define URDF_EDITOR_BASE_VIEW_MODEL_HH

#include <memory>
#include <urdf_model/link.h>

#include "urdf_editor/utils/urdf_clone.hh"

namespace urdf_editor {
namespace view_model {

template<typename M, typename Derived>
class BaseViewModel {
public:
    using ModelPtr = boost::shared_ptr<M>;
    using DerivedPtr = std::shared_ptr<Derived>;

public:
    explicit BaseViewModel(const ModelPtr &model) : model_(model) {
        if (!model_)
            model_.reset(new M());
    }

public:
    const ModelPtr &Model() const { return model_; }

    DerivedPtr Clone() const { return DerivedPtr(new Derived(utils::Clone(model_))); }

public:
    virtual void Sync() {}

protected:
    ModelPtr model_;
};

}
}

#endif //URDF_EDITOR_BASE_VIEW_MODEL_HH
