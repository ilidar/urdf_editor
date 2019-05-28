#include <utility>

//
// Created by Denys Kotelovych on 17.04.19.
//

#include <QMessageBox>

#include "urdf_editor/ui/add_link_dialog.hh"
#include "ui_add_link_dialog.h"

namespace urdf_editor {
namespace ui {

AddLinkDialog::AddLinkDialog(view_model::LinkViewModelPtr vm, QWidget *parent)
        : QDialog(parent), ui_(new Ui::AddLinkDialogUI()), vm_(std::move(vm)) {
    ui_->setupUi(this);

    ui_->JointParentLinkComboBox->addItems(vm_->Joint()->UsedLinkNames());
    if (!vm_->Joint()->UsedLinkNames().empty())
        vm_->Joint()->ParentLinkName(vm_->Joint()->UsedLinkNames().first());

    connect(ui_->LinkNameLineEdit, SIGNAL(textChanged(const QString &)), this,
            SLOT(LinkNameLineEditTextChanged(const QString &)));
    connect(ui_->JointNameLineEdit, SIGNAL(textChanged(const QString &)), this,
            SLOT(JointNameLineEditTextChanged(const QString &)));
    connect(ui_->JointParentLinkComboBox, SIGNAL(currentIndexChanged(int)), this,
            SLOT(JointParentComboBoxIndexChanged(int)));
}

void AddLinkDialog::done(int code) {
    if (code == QDialog::Rejected) {
        QDialog::done(code);
        return;
    }

    if (!vm_->Valid()) {
        QMessageBox::warning(this, "Error", "No link name specified or name already exist",
                QMessageBox::Cancel, QMessageBox::Cancel);
        return;
    }

    QDialog::done(code);
}

void AddLinkDialog::LinkNameLineEditTextChanged(const QString &text) {
    vm_->Name(text);
    vm_->Joint()->ChildLinkName(text);
    vm_->Joint()->GenerateName();
    vm_->Sync();

    ui_->JointNameLineEdit->setText(vm_->Joint()->Name());
}

void AddLinkDialog::JointNameLineEditTextChanged(const QString &text) {
    vm_->Joint()->Name(text);
    vm_->Sync();
}

void AddLinkDialog::JointParentComboBoxIndexChanged(int) {
    vm_->Joint()->ParentLinkName(ui_->JointParentLinkComboBox->currentText());
    vm_->Joint()->GenerateName();
    vm_->Sync();

    ui_->JointNameLineEdit->setText(vm_->Joint()->Name());
}

}
}