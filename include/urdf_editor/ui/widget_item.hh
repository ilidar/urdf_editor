//
// Created by Denys Kotelovych on 26.03.19.
//

#ifndef URDF_EDITOR_WIDGET_ITEM_HH
#define URDF_EDITOR_WIDGET_ITEM_HH

#include <QTreeWidgetItem>
#include <QListWidgetItem>

#include "urdf_editor/view_model/visual_view_model.hh"
#include "urdf_editor/view_model/collision_view_model.hh"
#include "urdf_editor/view_model/link_view_model.hh"

namespace urdf_editor {
namespace ui {

class LinkTreeWidgetItem : public QTreeWidgetItem {
public:
    explicit LinkTreeWidgetItem(view_model::LinkViewModelPtr vm, QTreeWidget *treeWidget = nullptr)
            : QTreeWidgetItem(treeWidget, QTreeWidgetItem::Type), vm_(std::move(vm)) {}

public:
    const view_model::LinkViewModelPtr &ViewModel() const { return vm_; }

private:
    view_model::LinkViewModelPtr vm_;
};

class VisualListWidgetItem : public QListWidgetItem {
public:
    explicit VisualListWidgetItem(const view_model::VisualViewModelPtr &vm)
            : QListWidgetItem(vm->Name()), vm_(vm) {}

public:
    const view_model::VisualViewModelPtr &ViewModel() const { return vm_; }

private:
    view_model::VisualViewModelPtr vm_;
};

class CollisionListWidgetItem : public QListWidgetItem {
public:
    explicit CollisionListWidgetItem(const view_model::CollisionViewModelPtr &vm)
            : QListWidgetItem(vm->Name()), vm_(vm) {}

public:
    const view_model::CollisionViewModelPtr &ViewModel() const { return vm_; }

private:
    view_model::CollisionViewModelPtr vm_;
};

}
}

#endif //URDF_EDITOR_WIDGET_ITEM_HH
