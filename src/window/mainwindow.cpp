#include <QDate>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QUrl>
#include "dialog/preferencesdialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

namespace PixelMaestroStudio {
	/*
	 * TODO: Add ability to add/remove Maestro preview widget and window on-demand
	 * Look into dockable widgets: https://doc.qt.io/qt-5/qdockwidget.html
	 */
	MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
		ui->setupUi(this);
		setWindowTitle(QCoreApplication::applicationName());

		// Configure split view
		QLayout* main_layout = this->findChild<QLayout*>("mainLayout");
		this->splitter_ = new QSplitter(main_layout->widget());
		this->splitter_->setOrientation(Qt::Orientation::Vertical);
		main_layout->addWidget(this->splitter_);

		// If the user has a session saved and they chose to continue from their last session, open the session, otherwise start a new session
		QSettings settings;
		QByteArray bytes = settings.value(PreferencesDialog::last_session).toByteArray();

		// Restore window size
		this->restoreGeometry(settings.value(PreferencesDialog::window_geometry).toByteArray());
		this->restoreState(settings.value(PreferencesDialog::window_state).toByteArray());

		// Restore splitter position
		this->splitter_->restoreState(settings.value(PreferencesDialog::splitter_position).toByteArray());

		// Restore previous Maestro session
		if (settings.value(PreferencesDialog::save_session).toBool() == true && !bytes.isEmpty()) {
			open_cuefile(bytes, true);
		}
		else {
			// Open the Animation Editor
			on_action_Open_Animation_Editor_triggered(false);
		}
	}

	void MainWindow::on_action_About_triggered() {
		QMessageBox::about(this, QString(QCoreApplication::applicationName()),
					   QString("Build ") + QString(BUILD_VERSION) +
					   QString("\n\nPixelMaestro is a library for creating and rendering 2D animations and patterns.") +
					   QString("\n\n© ") +
					   QString::number(QDate::currentDate().year()));
	}

	/**
	 * Closes the program.
	 */
	void MainWindow::on_action_Exit_triggered() {
		close();
	}

	/**
	 * Opens the documentation site in a browser.
	 */
	void MainWindow::on_action_Online_Help_triggered() {
		QDesktopServices::openUrl(QUrl("https://github.com/8bitbuddhist/PixelMaestro-Studio/wiki", QUrl::TolerantMode));
	}

	/**
	 * Opens an Animation Editor instance.
	 * @param keep_current_open If true, uses the existing DrawingArea instead of creating a new one. This is needed when loading in Cuefiles.
	 */
	void MainWindow::on_action_Open_Animation_Editor_triggered(bool keep_current_open) {

		// If Animation Editor is currently open, verify user wants to close
		if (maestro_control_widget_ != nullptr && !keep_current_open) {
			QMessageBox::StandardButton confirm;
			confirm = QMessageBox::question(this, "Open new Maestro", "Your current settings will be lost. Are you sure you want to continue?", QMessageBox::Yes|QMessageBox::No);
			if (confirm != QMessageBox::Yes) {
				return;
			}
		}

		QSettings settings;

		if (!keep_current_open) {
			initialize_widgets();
		}

		// If the Main DrawingArea is enabled as an output device, display it
		if (settings.value(PreferencesDialog::main_window_option, true) == true && maestro_drawing_area_ == nullptr) {
			maestro_drawing_area_ = new MaestroDrawingArea(splitter_, maestro_controller_);
			splitter_->addWidget(maestro_drawing_area_);
		}

		maestro_controller_->start();

		// Initialize MaestroControlWidget
		if (maestro_control_widget_ == nullptr) {
			maestro_control_widget_ = new MaestroControlWidget(splitter_, maestro_controller_);
			splitter_->addWidget(maestro_control_widget_);
		}

		// Resize split view so that it's 50/50
		this->splitter_->setSizes(QList<int>({INT_MAX, INT_MAX}));

		if (maestro_drawing_area_ != nullptr) {
			static_cast<MaestroDrawingArea*>(maestro_drawing_area_)->set_maestro_control_widget(maestro_control_widget_);
		}

		ui->action_Save_Maestro->setEnabled(true);
	}

	/**
	 * Merges a Cuefile into the current Maestro.
	 */
	void MainWindow::on_action_Open_triggered() {
		open_cuefile(open_cuefile_dialog(), false);
	}

	/**
	 * Replaces the current Maestro with the loaded Cuefile.
	 */
	void MainWindow::on_actionOpen_Maestro_triggered() {
		QString file = open_cuefile_dialog();
		open_cuefile(file, true);
		set_active_cuefile(file);
	}

	/**
	 * Opens the Preferences dialog.
	 */
	void MainWindow::on_action_Preferences_triggered() {
		PreferencesDialog preferences;
		preferences.exec();
	}

	/**
	 * Saves the current Maestro to a Cuefile.
	 */
	void MainWindow::on_action_Save_triggered() {
		if (this->active_cuefile_.isEmpty()) {
			on_action_Save_Maestro_triggered();
		}
		else {
			QSettings settings;
			// Store the directory that the file was saved to
			settings.setValue(PreferencesDialog::last_cuefile_directory, QFileInfo(this->active_cuefile_).path());
			maestro_controller_->save_cuefile(this->active_cuefile_);
		}
	}

	/**
	 * Saves the current Maestro to a new Cuefile.
	 */
	void MainWindow::on_action_Save_Maestro_triggered() {
		// Open the window in the last used directory, if possible
		QSettings settings;
		QString path = settings.value(PreferencesDialog::last_cuefile_directory, QDir::home().path()).toString();
		QString filename = QFileDialog::getSaveFileName(this,
			QString("Save Cue File"),
			path,
			QString("PixelMaestro Cue File (*.pmc)"));

		if (!filename.isEmpty()) {
			on_action_Save_triggered();
		}
	}

	/**
	 * Prompts the user to select a Cuefile.
	 * @return Cuefile filename.
	 */
	QString MainWindow::open_cuefile_dialog() {
		// Open the window in the last used directory, if possible
		QSettings settings;
		QString path = settings.value(PreferencesDialog::last_cuefile_directory, QDir::home().path()).toString();
		QString filename = QFileDialog::getOpenFileName(this,
			QString("Open Cue File"),
			path,
			QString("PixelMaestro Cue File (*.pmc)"));

		// Store the directory that the file was opened from
		settings.setValue(PreferencesDialog::last_cuefile_directory, QFileInfo(filename).path());
		return filename;
	}

	/**
	 * Reinitializes all widgets.
	 */
	void MainWindow::initialize_widgets() {
		removeEventFilter(maestro_drawing_area_);

		ui->action_Save_Maestro->setEnabled(false);

		if (maestro_drawing_area_) {
			int drawing_area_index = splitter_->indexOf(maestro_drawing_area_);
			if (drawing_area_index >= 0) {
				splitter_->widget(drawing_area_index)->hide();
				splitter_->widget(drawing_area_index)->deleteLater();
			}

			delete maestro_drawing_area_;
			maestro_drawing_area_ = nullptr;
		}
		if (maestro_control_widget_) {
			int control_widget_index = splitter_->indexOf(maestro_control_widget_);
			if (control_widget_index >= 0) {
				splitter_->widget(control_widget_index)->hide();
				splitter_->widget(control_widget_index)->deleteLater();
			}

			delete maestro_control_widget_;
			maestro_control_widget_ = nullptr;
		}
		if (maestro_controller_) {
			delete maestro_controller_;
			maestro_controller_ = nullptr;
		}

		maestro_controller_ = new MaestroController();
	}

	/**
	 * Loads a Cuefile into a new session.
	 * @param byte_array Array containing the Cuefile.
	 */
	void MainWindow::open_cuefile(QByteArray byte_array, bool new_session) {
		// Read in the Cuefile, then load the Animation Editor
		on_action_Open_Animation_Editor_triggered(!new_session);

		maestro_control_widget_->load_cuefile(byte_array);

		// Refresh the Animation Editor
		maestro_control_widget_->refresh_maestro_settings();
		maestro_control_widget_->set_active_section(maestro_control_widget_->get_active_section());
	}

	/**
	 * Loads a Cuefile into a new session.
	 * @param filename Cuefile path.
	 */
	void MainWindow::open_cuefile(QString filename, bool new_session) {
		if (filename.isEmpty()) return;

		QFile file(filename);
		if (file.open(QFile::ReadOnly)) {
			QByteArray bytes = file.readAll();
			open_cuefile(bytes, new_session);
		}
	}

	/**
	 * Sets the current active Cuefile to the path specified.
	 *
	 * @param path Path to the current Cuefile.
	 */
	void MainWindow::set_active_cuefile(QString path) {
		this->active_cuefile_ = path;

		if (!path.isEmpty()) {
			this->setWindowTitle("PixelMaestro Studio - " + QFileInfo(path).fileName());
		}
		else {
			this->setWindowTitle("PixelMaestro Studio");
		}
	}

	MainWindow::~MainWindow() {
		QSettings settings;

		// Save window geometry
		settings.setValue(PreferencesDialog::window_geometry, saveGeometry());
		settings.setValue(PreferencesDialog::window_state, saveState());

		// Save splitter position
		settings.setValue(PreferencesDialog::splitter_position, this->splitter_->saveState());

		// Save session (if enabled)
		if (settings.value(PreferencesDialog::save_session).toBool() == true) {
			// Save Maestro config
			QByteArray maestro_config;
			QDataStream maestro_datastream(&maestro_config, QIODevice::Truncate);
			if (maestro_controller_ != nullptr) {
				maestro_controller_->save_maestro_to_datastream(&maestro_datastream);
			}
			settings.setValue(PreferencesDialog::last_session, maestro_config);
		}

		delete maestro_control_widget_;
		delete splitter_;
		delete ui;
	}
}
