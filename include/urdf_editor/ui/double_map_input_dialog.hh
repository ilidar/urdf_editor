//
// Created by Denys Kotelovych on 17.04.19.
//

#ifndef URDF_EDITOR_DOUBLE_MAP_INPUT_DIALOG_HH
#define URDF_EDITOR_DOUBLE_MAP_INPUT_DIALOG_HH

#include <map>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QLabel>

namespace urdf_editor {
namespace ui {

class DoubleMapInputDialog : public QDialog {
Q_OBJECT
public:
    explicit DoubleMapInputDialog(const QStringList &fieldNames, QWidget *parent = nullptr)
            : QDialog(parent) {
        auto rootLayout = new QVBoxLayout;
        for (const auto &fieldName : fieldNames) {
            auto layout = new QHBoxLayout;
            auto label = new QLabel;
            label->setText(fieldName);
            layout->addWidget(label);
            auto spinBox = new QDoubleSpinBox;
            spinBox->setMaximum(1000.0);
            spinBox->setMinimum(0.0);
            spinBox->setSingleStep(0.1);
            spinBox->setDecimals(6);
            layout->addWidget(spinBox);
            rootLayout->addLayout(layout);

            connect(spinBox, SIGNAL(valueChanged(double)), this, SLOT(SpinBoxValueChanged(double)));
            spinBoxToFieldNameMap_[spinBox] = fieldName;
        }

        auto buttonBox = new QDialogButtonBox;
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel | QDialogButtonBox::Ok);
        rootLayout->addWidget(buttonBox);
        setLayout(rootLayout);

        QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    }

public:
    const std::map<QString, double> &Map() const { return fieldNameToValueMap_; }

private Q_SLOTS:

    void SpinBoxValueChanged(double value) {
        auto obj = sender();
        auto fieldName = spinBoxToFieldNameMap_[dynamic_cast<QDoubleSpinBox *>(obj)];
        fieldNameToValueMap_[fieldName] = value;
    }

private:
    std::map<QString, double> fieldNameToValueMap_;
    std::map<QDoubleSpinBox *, QString> spinBoxToFieldNameMap_;
};

}
}

#endif //URDF_EDITOR_DOUBLE_MAP_INPUT_DIALOG_HH
