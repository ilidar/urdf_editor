//
// Created by Denys Kotelovych on 29.03.19.
//

#include "urdf_editor/view_model/geometry_view_model.hh"

static const std::map<GeometryType, QString> kGeometryTypeToNameMap = {
        {urdf::Geometry::BOX,      "Box"},
        {urdf::Geometry::SPHERE,   "Sphere"},
        {urdf::Geometry::CYLINDER, "Cylinder"},
        {urdf::Geometry::MESH,     "Mesh"},
};

static const std::map<QString, GeometryType> kGeometryNameToTypeMap = {
        {"Box",      urdf::Geometry::BOX},
        {"Sphere",   urdf::Geometry::SPHERE},
        {"Cylinder", urdf::Geometry::CYLINDER},
        {"Mesh",     urdf::Geometry::MESH},
};

namespace urdf_editor {
namespace view_model {

const QString &GeometryViewModel::Name() const {
    return kGeometryTypeToNameMap.at(type_);
}

void GeometryViewModel::Type(const QString &type) {
    type_ = kGeometryNameToTypeMap.at(type);
}

void GeometryViewModel::Sync() {
    switch (type_) {
        case GeometryType::BOX: {
            auto box = new urdf::Box();
            box->dim.x = length_;
            box->dim.y = width_;
            box->dim.z = height_;
            model_.reset(box);
            break;
        }
        case GeometryType::CYLINDER: {
            auto cylinder = new urdf::Cylinder();
            cylinder->length = length_;
            cylinder->radius = radius_;
            model_.reset(cylinder);
            break;
        }
        case GeometryType::SPHERE: {
            auto sphere = new urdf::Sphere();
            sphere->radius = radius_;
            model_.reset(sphere);
            break;
        }
        case GeometryType::MESH: {
            auto mesh = new urdf::Mesh();
            mesh->filename = filepath_;
            model_.reset(mesh);
            break;
        }
        default:
            assert(false);
            break;
    }
}

void GeometryViewModel::Load() {
    radius_ = 0.0;
    length_ = 0.0;
    width_ = 0.0;
    height_ = 0.0;
    filepath_ = "";

    switch (type_) {
        case GeometryType::BOX: {
            auto box = urdf::dynamic_pointer_cast<urdf::Box>(model_);
            length_ = box->dim.x;
            width_ = box->dim.y;
            height_ = box->dim.z;
            break;
        }
        case GeometryType::CYLINDER: {
            auto cylinder = urdf::dynamic_pointer_cast<urdf::Cylinder>(model_);
            length_ = cylinder->length;
            radius_ = cylinder->radius;
            break;
        }
        case GeometryType::SPHERE: {
            auto box = urdf::dynamic_pointer_cast<urdf::Sphere>(model_);
            radius_ = box->radius;
            break;
        }
        case GeometryType::MESH: {
            auto mesh = urdf::dynamic_pointer_cast<urdf::Mesh>(model_);
            filepath_ = mesh->filename;
            break;
        }
        default:
            assert(false);
            break;
    }
}

}
}