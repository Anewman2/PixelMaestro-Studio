#ifndef SPARKLEANIMATIONCONTROLWIDGET_H
#define SPARKLEANIMATIONCONTROLWIDGET_H

#include <QWidget>
#include "animation/sparkleanimation.h"
#include "widget/maestrocontrolwidget.h"

using namespace PixelMaestro;

namespace Ui {
	class SparkleAnimationControlWidget;
}

namespace PixelMaestroStudio {
	class SparkleAnimationControlWidget : public QWidget {
			Q_OBJECT

		public:
			explicit SparkleAnimationControlWidget(SparkleAnimation* animation, MaestroControlWidget* controller, QWidget *parent = 0);
			~SparkleAnimationControlWidget();

		private slots:
			void on_thresholdSpinBox_valueChanged(int arg1);

		private:
			SparkleAnimation* animation_;
			MaestroControlWidget* maestro_control_;
			Ui::SparkleAnimationControlWidget *ui;
	};
}

#endif // SPARKLEANIMATIONCONTROLWIDGET_H
