#include "GwScrollArea.h"
#include <QPixmap>
#include <QImage>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QMessageBox>

GwScrollArea::GwScrollArea(QWidget *parent)
	:QScrollArea(parent)
{
	mHBoxMainLayout = new QHBoxLayout(parent);
	this->installEventFilter(this);
	this->setAcceptDrops(true);
	this->setStyleSheet(
		"GwScrollArea{"
		"font-size:20px;"
		"border:2px dashed gray;"
		"border-radius:10px;"
		"background:rgba(25,25,25,25);"
		"}");
	mHBoxMainLayout->setParent(this);
	mHBoxMainLayout->setAlignment(Qt::AlignLeft);

	this->setLayout(mHBoxMainLayout);
	
}


GwScrollArea::~GwScrollArea()
{
}

bool GwScrollArea::eventFilter(QObject *watched, QEvent *event) {

	if (watched == this) {
		if (event->type() == QEvent::DragEnter) {

			// [[2]]: ���Ϸ�ʱ������labelʱ, label�����ϷŵĶ���
			QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);

			dee->acceptProposedAction();

			return true;

		}
		else if (event->type() == QEvent::Drop) {
			// [[3]]: ���Ų���������, ȡ���Ϸŵ�����
			QDropEvent *de = dynamic_cast<QDropEvent *>(event);
			QList<QUrl> urls = de->mimeData()->urls();
			if (urls.isEmpty()) {
				return true;
			}
			QString path = urls.first().toLocalFile();
			// [[4]]: ��label����ʾ�Ϸŵ�ͼƬ
			QImage image(path); // QImage��I/O�Ż���, QPixmap����ʾ�Ż�

			if (!image.isNull()) {
				image = image.scaled(childLabelHeight, childLabelWidth,
					Qt::KeepAspectRatio,
					Qt::SmoothTransformation);

				QLabel *tmpLabel = new QLabel(this);
				tmpLabel->setPixmap(QPixmap::fromImage(image));
				this->mLabelList.push_back(tmpLabel);
				this->mHBoxMainLayout->addWidget(tmpLabel);
				//this->setWidget(tmpLabel);
			}
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}

void GwScrollArea::setInputImagesList(QStringList imgs)
{
	this->mImageList = imgs;
}

void GwScrollArea::update()
{
	for (int i = 0; i < mImageList.size(); i++) {

		QLabel *tmpLabel = new QLabel;
		QImage *tmpImg = new QImage;
		tmpImg->load(mImageList[i]);
		tmpLabel->setFixedSize(childLabelWidth, childLabelHeight);
		tmpLabel->setPixmap(QPixmap::fromImage(*tmpImg)
			.scaledToHeight(tmpLabel->height())
			.scaledToWidth(tmpLabel->width()));
		tmpLabel->setAlignment(Qt::AlignLeft);
		this->mLabelList.push_back(tmpLabel);

		//connect(tmpLabel, SIGNAL(clicked(bool)), this, SLOT(childLabelClicked));

		this->mHBoxMainLayout->addWidget(tmpLabel);

	}
}
