//
// Created by Denys Kotelovych on 29.03.19.
//

#ifndef URDF_EDITOR_GEOMETRY_VIEW_MODEL_HH
#define URDF_EDITOR_GEOMETRY_VIEW_MODEL_HH

#include <QString>

#include "urdf_editor/view_model/base_view_model.hh"

using GeometryType = decltype(urdf::Geometry::type);

namespace urdf_editor {
namespace view_model {

class GeometryViewModel : public BaseViewModel<urdf::Geometry, GeometryViewModel> {
public:
    explicit GeometryViewModel(const urdf::GeometrySharedPtr &model)
            : BaseViewModel<urdf::Geometry, GeometryViewModel>(model),
              type_(model_->type),
              radius_(0.0),
              length_(0.0),
              width_(0.0),
              height_(0.0) {
        if (!model)
            model_.reset(new urdf::Sphere());
        Load();
    }

public:
    const QString &Name() const;

    GeometryType Type() const { return type_; }

    void Type(GeometryType type) { type_ = type; }

    void Type(const QString &type);

    double Width() const { return width_; }

    void Width(double width) { width_ = width; }

    double Length() const { return length_; }

    void Length(double length) { length_ = length; }

    double Height() const { return height_; }

    void Height(double height) { height_ = height; }

    double Radius() const { return radius_; }

    void Radius(double radius) { radius_ = radius; }

    QString Filepath() const { return QString::fromStdString(filepath_); }

    void Filepath(const QString &filepath) { filepath_ = filepath.toStdString(); }

public:
    void Sync() override;

private:
    void Load();

private:
    GeometryType type_;
    double width_, length_, height_, radius_;
    std::string filepath_;
};

using GeometryViewModelPtr = std::shared_ptr<GeometryViewModel>;

}
}

#endif //URDF_EDITOR_GEOMETRY_VIEW_MODEL_HH
