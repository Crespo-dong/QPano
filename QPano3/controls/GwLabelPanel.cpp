#include "GwLabelPanel.h"
#include <QPixmap>
#include <QImage>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QList>
#include <QUrl>
#include <QMimeData>
#include <QMessageBox>
#include <QPushButton>

GwLabelPanel::GwLabelPanel(QWidget *parent)
	: QLabel(parent)
{
	this->setText("");
	mHBoxMainLayout = new QHBoxLayout(this);
	mMotherMainLayout = new QHBoxLayout(this);
	mWidget = new QWidget(this);

	mScrollArea = new QScrollArea(this);
	mScrollArea->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
	mScrollArea->setWidgetResizable(true);
	
	this->setMinimumHeight(thisWidgetHeight);
	this->installEventFilter(this);
	this->setAcceptDrops(true);	
	this->setScaledContents(true);
	this->setStyleSheet(
		"QScrollArea{"
		"font-size:20px;"
		"border:2px dashed gray;"
		"border-radius:10px;"
		"background:rgba(25,25,25,25);"
		"}");
 	mHBoxMainLayout->setAlignment(Qt::AlignLeft);	
	//here lose this code
	mHBoxMainLayout->setSizeConstraint(QHBoxLayout::SetMinAndMaxSize);

	
	this->mWidget->setLayout(mHBoxMainLayout);
	mScrollArea->setWidget(mWidget);

	mMotherMainLayout->addWidget(mScrollArea);
	this->setLayout(mMotherMainLayout);
}

GwLabelPanel::~GwLabelPanel()
{

}

bool GwLabelPanel::eventFilter(QObject *watched, QEvent *event) {

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
			else {
				for (int i=0;i<urls.size();i++){
					mImageList.push_back(urls[i].toLocalFile());
				}
			}
			this->update();
// 			QString path = urls.first().toLocalFile();
// 			// [[4]]: ��label����ʾ�Ϸŵ�ͼƬ
// 			QImage image(path); // QImage��I/O�Ż���, QPixmap����ʾ�Ż�
// 
// 			if (!image.isNull()) {
// 				image = image.scaled(childImageWidth, childImageHeight,
// 					Qt::KeepAspectRatio,
// 					Qt::SmoothTransformation);
// 
// 				QLabel *tmpLabel = getInsertImageLabel(image);
// 				this->mLabelList.push_back(tmpLabel);
// 				//this->mHBoxMainLayout->addWidget(tmpLabel);	
// 				this->mHBoxMainLayout->addWidget(tmpLabel);
// 			}
			return true;
		}
	}
	return QWidget::eventFilter(watched, event);
}
void GwLabelPanel::setInputImagesList(QStringList imgs)
{
	mImageList = imgs;
}

void GwLabelPanel::update()
{	
	while (mImageList.size()){
		QLabel *tmpLabel = new QLabel;
		QImage tmpImg(mImageList.front());
		mImageList.pop_front();
		tmpLabel = getInsertImageLabel(tmpImg);
		this->mLabelList.push_back(tmpLabel);

		this->mHBoxMainLayout->addWidget(tmpLabel);
	}
}

QLabel* GwLabelPanel::getInsertImageLabel(QImage image)
{
	QLabel *tmpLabel = new QLabel(this);
	tmpLabel->setFixedSize(childLabelWidth ,childLabelHeight);
	tmpLabel->setPixmap(QPixmap::fromImage(image)
		.scaledToHeight(childImageWidth)
		.scaledToWidth(childImageHeight)
	);
	return tmpLabel;
}

void GwLabelPanel::childLabelClicked(QStringList imgs, int index)
{
	QMessageBox::information(NULL, "error", "Stitcher result is error");
}
