//
// Created by Denys Kotelovych on 30.03.19.
//

#ifndef URDF_EDITOR_MATERIAL_VIEW_MODEL_HH
#define URDF_EDITOR_MATERIAL_VIEW_MODEL_HH

#include <QColor>

#include "urdf_editor/view_model/base_view_model.hh"
#include "urdf_editor/utils/time.hh"

namespace urdf_editor {
namespace view_model {

class MaterialViewModel : public BaseViewModel<urdf::Material, MaterialViewModel> {
public:
    explicit MaterialViewModel(const urdf::MaterialSharedPtr &model)
            : BaseViewModel<urdf::Material, MaterialViewModel>(model) {
        if (model_->name.empty())
            model_->name = "material_" + std::to_string(utils::TimeNowMilliseconds());
    }

public:
    QString Name() const { return QString::fromStdString(model_->name); }

    void Name(const QString &name) { model_->name = name.toStdString(); }

    const urdf::Color &Color() const { return model_->color; }

    void Color(double r, double g, double b, double a) {
        model_->color.r = static_cast<float>(r);
        model_->color.g = static_cast<float>(g);
        model_->color.b = static_cast<float>(b);
        model_->color.a = static_cast<float>(a);
    }

    void Color(const QColor &color) {
        Color(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    }

    QString TextureFileName() const { return QString::fromStdString(model_->texture_filename); }

    void TextureFileName(const QString &filename) { model_->texture_filename = filename.toStdString(); }
};

using MaterialViewModelPtr = std::shared_ptr<MaterialViewModel>;

}
}

#endif //URDF_EDITOR_MATERIAL_VIEW_MODEL_HH
