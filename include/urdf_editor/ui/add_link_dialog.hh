//
// Created by Denys Kotelovych on 17.04.19.
//

#ifndef URDF_EDITOR_ADD_LINK_DIALOG_HH
#define URDF_EDITOR_ADD_LINK_DIALOG_HH

#include <QDialog>

#include "urdf_editor/view_model/link_view_model.hh"

namespace Ui {
class AddLinkDialogUI;

using AddLinkDialogUIPtr = std::shared_ptr<AddLinkDialogUI>;
}

namespace urdf_editor {
namespace ui {

class AddLinkDialog : public QDialog {
Q_OBJECT
public:
    explicit AddLinkDialog(view_model::LinkViewModelPtr vm, QWidget *parent = nullptr);

public:
    void done(int) override;

private Q_SLOTS:
    void LinkNameLineEditTextChanged(const QString &text);

    void JointNameLineEditTextChanged(const QString &text);

    void JointParentComboBoxIndexChanged(int index);

private:
    Ui::AddLinkDialogUIPtr ui_;
    view_model::LinkViewModelPtr vm_;
};

}
}

#endif //URDF_EDITOR_ADD_LINK_DIALOG_HH
