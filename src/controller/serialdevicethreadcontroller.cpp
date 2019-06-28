#include <QByteArray>
#include "serialdevicethreadcontroller.h"

namespace PixelMaestroStudio {
	SerialDeviceThreadController::SerialDeviceThreadController(SerialDeviceController& serial_device, const char *out, int size) : QThread(nullptr), serial_device_(serial_device) {
		this->output_.append(out, size);
	}

	void SerialDeviceThreadController::run() {
		/*
		 * How this works:
		 *
		 * When sending data to an Arduino, the Arduino might fail to process large chunks even at a low baud rate.
		 * This tends to happen at the 64 byte mark.
		 * As a workaround, we break up the output into 64 byte chunks and give the Arduino some time between chunks to catch up.
		 *
		 */

		emit progress_changed(0);

		int current_index = 0;
		int chunk_index = CHUNK_SIZE;

		do {
			if (current_index > 0) {
				msleep(SLEEP_INTERVAL);
			}

			QByteArray out_addr = output_.mid(current_index, chunk_index);

			if (current_index + chunk_index > output_.size()) {
				chunk_index = output_.size() - current_index;
			}
			serial_device_.get_device()->write(out_addr);
			serial_device_.get_device()->flush();
			current_index += chunk_index;
			emit progress_changed((current_index / (float)output_.size()) * 100);
		}
		while (current_index < output_.size());

		emit progress_changed(100);
	}
}
