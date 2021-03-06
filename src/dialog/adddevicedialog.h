#ifndef ADDDEVICEDIALOG_H
#define ADDDEVICEDIALOG_H

#include <QDialog>
#include <QVector>
#include "controller/devicecontroller.h"

namespace Ui {
	class AddDeviceDialog;
}

namespace PixelMaestroStudio {
	class AddDeviceDialog : public QDialog {
			Q_OBJECT

		public:
			explicit AddDeviceDialog(QVector<DeviceController>* devices, DeviceController* device, QWidget *parent = 0);
			~AddDeviceDialog();

		private slots:
			void on_buttonBox_accepted();

			void on_mapSectionsButton_clicked();

			void on_liveUpdatesCheckBox_stateChanged(int arg1);

		private:
			QVector<DeviceController>* devices_;
			DeviceController* device_;
			Ui::AddDeviceDialog *ui;

			bool is_device_already_added(QString port_name);
			void populate_serial_devices();
	};
}

#endif // ADDDEVICEDIALOG_H
