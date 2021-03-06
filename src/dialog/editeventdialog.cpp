#include <QIcon>
#include <QTime>
#include "editeventdialog.h"
#include "ui_editeventdialog.h"
#include "utility/cueinterpreter.h"

namespace PixelMaestroStudio {
	EditEventDialog::EditEventDialog(Event& event, QWidget *parent) : QDialog(parent), ui(new Ui::EditEventDialog), event_(event) {
		ui->setupUi(this);

		setWindowIcon(QIcon("qrc:/../../../docsrc/images/logo.png"));

		ui->cueLineEdit->setText(CueInterpreter::interpret_cue(event.get_cue()));

		QTime event_time = QTime::fromMSecsSinceStartOfDay(event.get_time());
		ui->timeEdit->setTime(event_time);
		ui->timeEdit->setFocus();
	}

	void EditEventDialog::on_buttonBox_accepted() {
		uint32_t new_time = ui->timeEdit->time().msecsSinceStartOfDay();

		if (new_time != event_.get_time()) {
			event_.set_time(new_time);
		}
	}

	EditEventDialog::~EditEventDialog() {
		delete ui;
	}
}
