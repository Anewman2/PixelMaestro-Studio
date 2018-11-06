#include <QWidget>
#include "animationcontrolwidget.h"
#include "ui_animationcontrolwidget.h"
#include "animation/animation.h"
#include "controller/palettecontroller.h"
#include "widget/animation/fireanimationcontrolwidget.h"
#include "widget/animation/lightninganimationcontrolwidget.h"
#include "widget/animation/plasmaanimationcontrolwidget.h"
#include "widget/animation/radialanimationcontrolwidget.h"
#include "widget/animation/sparkleanimationcontrolwidget.h"
#include "widget/animation/waveanimationcontrolwidget.h"

namespace PixelMaestroStudio {
	AnimationControlWidget::AnimationControlWidget(QWidget *parent) : QWidget(parent), ui(new Ui::AnimationControlWidget) {
		ui->setupUi(this);
		this->maestro_control_widget = static_cast<MaestroControlWidget*>(parent);
	}

	void AnimationControlWidget::initialize() {
		// Disable advanced settings by default
		ui->advancedSettingsGroupBox->setVisible(false);
	}

	/**
	 * Changes the current animation.
	 * @param index Index of the new animation.
	 */
	void AnimationControlWidget::on_typeComboBox_currentIndexChanged(int index) {
		// If the animation is set to "None", remove it
		if (index == 0) {
			maestro_control_widget->run_cue(
				maestro_control_widget->section_handler->remove_animation(
					maestro_control_widget->section_control_widget_->get_section_index(),
					maestro_control_widget->section_control_widget_->get_layer_index(),
					true
				)
			);
		}
		else {
			// If the Section has an active Animation, update it
			Animation* animation = maestro_control_widget->section_control_widget_->get_active_section()->get_animation();
			if (animation != nullptr) {
				// First, check to see if the Animation types match. If so, do nothing
				if (animation->get_type() == (AnimationType)(index - 1)) {
					return;
				}

				// Otherwise, replace the Animation, preserving options
				maestro_control_widget->run_cue(
					maestro_control_widget->section_handler->set_animation(
						maestro_control_widget->section_control_widget_->get_section_index(),
						maestro_control_widget->section_control_widget_->get_layer_index(),
						(AnimationType)(index - 1),
						true
					)
				);
			}
			else {	// No prior Aniamtion: set the new Animation and apply settings
				maestro_control_widget->run_cue(
					maestro_control_widget->section_handler->set_animation(
						maestro_control_widget->section_control_widget_->get_section_index(),
						maestro_control_widget->section_control_widget_->get_layer_index(),
						(AnimationType)(index - 1),
						false
					)
				);

				on_paletteComboBox_currentIndexChanged(ui->paletteComboBox->currentIndex());
				on_fadeCheckBox_toggled(ui->fadeCheckBox->isChecked());
				on_reverseCheckBox_toggled(ui->reverseCheckBox->isChecked());
				on_orientationComboBox_currentIndexChanged(ui->orientationComboBox->currentIndex());
				on_cycleIntervalSpinBox_editingFinished();
				on_delayIntervalSpinBox_editingFinished();
			}

			set_advanced_controls(maestro_control_widget->section_control_widget_->get_active_section()->get_animation());
		}

		set_controls_enabled(index > 0);
	}

	/**
	 * Sets the duration of each Animation cycle.
	 * @param value Cycle duration (in ms).
	 */
	void AnimationControlWidget::on_cycleIntervalSlider_valueChanged(int value) {
		ui->cycleIntervalSpinBox->blockSignals(true);
		ui->cycleIntervalSpinBox->setValue(value);
		ui->cycleIntervalSpinBox->blockSignals(false);

		set_animation_timer();
	}

	/**
	 * Sets the duration of each Animation cycle.
	 */
	void AnimationControlWidget::on_cycleIntervalSpinBox_editingFinished() {
		ui->cycleIntervalSlider->blockSignals(true);
		ui->cycleIntervalSlider->setValue(ui->cycleIntervalSpinBox->value());
		ui->cycleIntervalSlider->blockSignals(false);

		set_animation_timer();
	}

	/**
	 * Sets the time between the start of each Animation cycle.
	 * @param value Delay between cycles (in ms).
	 */
	void AnimationControlWidget::on_delayIntervalSlider_valueChanged(int value) {
		ui->delayIntervalSpinBox->blockSignals(true);
		ui->delayIntervalSpinBox->setValue(value);
		ui->delayIntervalSpinBox->blockSignals(false);

		set_animation_timer();
	}

	/**
	 * Sets the time between the start of each Animation cycle.
	 */
	void AnimationControlWidget::on_delayIntervalSpinBox_editingFinished() {
		ui->delayIntervalSlider->blockSignals(true);
		ui->delayIntervalSlider->setValue(ui->delayIntervalSpinBox->value());
		ui->delayIntervalSlider->blockSignals(false);

		set_animation_timer();
	}

	/**
	 * Toggles Animation fading.
	 * @param checked If true, fade between animation cycles.
	 */
	void AnimationControlWidget::on_fadeCheckBox_toggled(bool checked) {
		maestro_control_widget->run_cue(
			maestro_control_widget->animation_handler->set_fade(
				maestro_control_widget->section_control_widget_->get_section_index(),
				maestro_control_widget->section_control_widget_->get_layer_index(),
				checked
			)
		);
	}

	/**
	 * Sets the Animation's orientation.
	 * @param index New orientation
	 */
	void AnimationControlWidget::on_orientationComboBox_currentIndexChanged(int index) {
		// Check to see if the new orientation is different from the old one
		Animation* animation = maestro_control_widget->section_control_widget_->get_active_section()->get_animation();
		if (animation && animation->get_orientation() != (Animation::Orientation)index) {
			maestro_control_widget->run_cue(
				maestro_control_widget->animation_handler->set_orientation(
					maestro_control_widget->section_control_widget_->get_section_index(),
					maestro_control_widget->section_control_widget_->get_layer_index(),
					(Animation::Orientation)index
				)
			);
		}
	}

	/**
	 * Changes the Animation's Palette.
	 * @param index New Palette index.
	 */
	void AnimationControlWidget::on_paletteComboBox_currentIndexChanged(int index) {
		PaletteController::PaletteWrapper* palette_wrapper = maestro_control_widget->palette_controller_.get_palette(index);
		maestro_control_widget->run_cue(
			maestro_control_widget->animation_handler->set_palette(
				maestro_control_widget->section_control_widget_->get_section_index(),
				maestro_control_widget->section_control_widget_->get_layer_index(),
				&palette_wrapper->palette
			)
		);
	}

	void AnimationControlWidget::on_paletteEditButton_clicked() {
		maestro_control_widget->edit_palettes(ui->paletteComboBox->currentText());
	}

	/**
	 * Toggles the direction of the Animation.
	 * @param checked If true, run the Animation in reverse.
	 */
	void AnimationControlWidget::on_reverseCheckBox_toggled(bool checked) {
		maestro_control_widget->run_cue(
			maestro_control_widget->animation_handler->set_reverse(
				maestro_control_widget->section_control_widget_->get_section_index(),
				maestro_control_widget->section_control_widget_->get_layer_index(),
				checked
			)
		);
	}

	/**
	 * Updates the UI based on the active Section.
	 */
	void AnimationControlWidget::refresh() {
		Animation* animation = maestro_control_widget->section_control_widget_->get_active_section()->get_animation();

		// If there is no Animation, select 'None' and exit.
		if (animation == nullptr) {
			ui->typeComboBox->blockSignals(true);
			ui->typeComboBox->setCurrentIndex(0);
			ui->typeComboBox->blockSignals(false);
			return;
		}

		ui->orientationComboBox->blockSignals(true);
		ui->reverseCheckBox->blockSignals(true);
		ui->fadeCheckBox->blockSignals(true);
		ui->cycleIntervalSlider->blockSignals(true);
		ui->cycleIntervalSpinBox->blockSignals(true);
		ui->delayIntervalSlider->blockSignals(true);
		ui->delayIntervalSpinBox->blockSignals(true);
		ui->orientationComboBox->setCurrentIndex((uint8_t)animation->get_orientation());
		ui->reverseCheckBox->setChecked(animation->get_reverse());
		ui->fadeCheckBox->setChecked(animation->get_fade());
		ui->cycleIntervalSlider->setValue(animation->get_timer()->get_interval());
		ui->cycleIntervalSpinBox->setValue(animation->get_timer()->get_interval());
		ui->delayIntervalSlider->setValue(animation->get_timer()->get_delay());
		ui->delayIntervalSpinBox->setValue(animation->get_timer()->get_delay());
		ui->orientationComboBox->blockSignals(false);
		ui->reverseCheckBox->blockSignals(false);
		ui->fadeCheckBox->blockSignals(false);
		ui->cycleIntervalSlider->blockSignals(false);
		ui->cycleIntervalSpinBox->blockSignals(false);
		ui->delayIntervalSlider->blockSignals(false);
		ui->delayIntervalSpinBox->blockSignals(false);

		// Select the current Palette
		int palette_index = maestro_control_widget->palette_controller_.find(animation->get_palette()->get_colors());
		if (palette_index >= 0) {
			ui->paletteComboBox->blockSignals(true);
			ui->paletteComboBox->setCurrentIndex(palette_index);
			ui->paletteComboBox->blockSignals(false);
		}
		else {	// Palette not found
			QString name = "Section " + QString::number(maestro_control_widget->section_control_widget_->get_section_index()) +
						   " Layer " + QString::number(maestro_control_widget->section_control_widget_->get_layer_index()) +
						   " Animation";
			maestro_control_widget->palette_controller_.add_palette(name, animation->get_palette()->get_colors(), animation->get_palette()->get_num_colors(), PaletteController::PaletteType::Random, Colors::RGB(0, 0, 0), Colors::RGB(0, 0, 0), false);
			ui->paletteComboBox->blockSignals(true);
			ui->paletteComboBox->addItem(name);
			ui->paletteComboBox->setCurrentText(name);
			ui->paletteComboBox->blockSignals(false);
		}

		// Set the Animation type
		ui->typeComboBox->blockSignals(true);
		ui->typeComboBox->setCurrentIndex((uint8_t)animation->get_type() + 1);
		ui->typeComboBox->blockSignals(false);

		// Enable controls
		set_controls_enabled(true);
		set_advanced_controls(animation);
	}

	void AnimationControlWidget::refresh_palettes() {
		QString palette = ui->paletteComboBox->currentText();

		ui->paletteComboBox->blockSignals(true);
		ui->paletteComboBox->clear();

		for (uint16_t i = 0; i < maestro_control_widget->palette_controller_.get_palettes()->size(); i++) {
			ui->paletteComboBox->addItem(maestro_control_widget->palette_controller_.get_palette(i)->name);
		}

		ui->paletteComboBox->setCurrentText(palette);
		ui->paletteComboBox->blockSignals(false);
	}

	/**
	 * Displays extra controls for Animations that take custom parameters.
	 * @param animation New Animation.
	 */
	void AnimationControlWidget::set_advanced_controls(Animation *animation) {
		// First, remove any existing extra control widgets
		if (advanced_controls_widget_ != nullptr) {
			this->findChild<QLayout*>("advancedSettingsLayout")->removeWidget(advanced_controls_widget_.get());
			advanced_controls_widget_.reset();
		}

		QLayout* layout = this->findChild<QLayout*>("advancedSettingsLayout");

		switch(animation->get_type()) {
			case AnimationType::Fire:
				advanced_controls_widget_ = QSharedPointer<QWidget>(new FireAnimationControlWidget((FireAnimation*)animation, this->maestro_control_widget, layout->widget()));
				break;
			case AnimationType::Lightning:
				advanced_controls_widget_ = QSharedPointer<QWidget>(new LightningAnimationControlWidget((LightningAnimation*)animation, this->maestro_control_widget, layout->widget()));
				break;
			case AnimationType::Plasma:
				advanced_controls_widget_ = QSharedPointer<QWidget>(new PlasmaAnimationControlWidget((PlasmaAnimation*)animation, this->maestro_control_widget, layout->widget()));
				break;
			case AnimationType::Radial:
				advanced_controls_widget_ = QSharedPointer<QWidget>(new RadialAnimationControlWidget((RadialAnimation*)animation, this->maestro_control_widget, layout->widget()));
				break;
			case AnimationType::Sparkle:
				advanced_controls_widget_ = QSharedPointer<QWidget>(new SparkleAnimationControlWidget((SparkleAnimation*)animation, this->maestro_control_widget, layout->widget()));
				break;
			case AnimationType::Wave:
				advanced_controls_widget_ = QSharedPointer<QWidget>(new WaveAnimationControlWidget((WaveAnimation*)animation, this->maestro_control_widget, layout->widget()));
			default:
				break;
		}

		ui->advancedSettingsGroupBox->setVisible(advanced_controls_widget_ != nullptr);

		if (advanced_controls_widget_) {
			layout->addWidget(advanced_controls_widget_.get());
		}
	}

	/**
	 * Updates the Animation's Timer.
	 */
	void AnimationControlWidget::set_animation_timer() {
		Animation* animation = maestro_control_widget->section_control_widget_->get_active_section()->get_animation();

		if (animation == nullptr) return;

		uint16_t interval = ui->cycleIntervalSpinBox->value();
		uint16_t delay = ui->delayIntervalSpinBox->value();

		AnimationTimer* timer = animation->get_timer();
		if (interval != timer->get_interval() || delay != timer->get_delay()) {
			maestro_control_widget->run_cue(
				maestro_control_widget->animation_handler->set_timer(
					maestro_control_widget->section_control_widget_->get_section_index(),
					maestro_control_widget->section_control_widget_->get_layer_index(),
					interval,
					delay
				)
			);
		}
	}

	void AnimationControlWidget::set_controls_enabled(bool enabled) {
		ui->orientationComboBox->setEnabled(enabled);
		ui->reverseCheckBox->setEnabled(enabled);
		ui->paletteComboBox->setEnabled(enabled);
		ui->fadeCheckBox->setEnabled(enabled);
		ui->advancedSettingsGroupBox->setEnabled(enabled);
		ui->timersGroupBox->setEnabled(enabled);
	}

	AnimationControlWidget::~AnimationControlWidget() {
		delete ui;
	}
}