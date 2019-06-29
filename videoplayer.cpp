#include "videoplayer.h"

#include <QtWidgets>
#include <QVideoWidget>

#include <QFileDialog>
#include <QMediaMetaData>
#include <QFileInfo>

#include <QtGui>
#include <QUrl>



VideoPlayer::VideoPlayer(QWidget *parent)
    : QWidget(parent)
{

    m_mediaPlayer = new QMediaPlayer(this, QMediaPlayer::VideoSurface);
    QVideoWidget *vWidget = new QVideoWidget;

    QAbstractButton *open = new QPushButton;
    open->setIcon(style()->standardIcon(QStyle::SP_DialogOpenButton));
    connect(open, &QAbstractButton::clicked,
            this, &VideoPlayer::open);

    m_playButton = new QPushButton;
    m_playButton->setEnabled(false);
//    m_playButton->setText(tr("Play"));
    m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(m_playButton, &QAbstractButton::clicked,
            this, &VideoPlayer::play);

    exitButton = new QPushButton;
    exitButton->setText(tr("Exit"));

    connect(exitButton, &QAbstractButton::clicked,
            this, &QApplication::quit);


    mute = new QPushButton;
//    mute->setText("Mute");
    mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    connect(mute, &QAbstractButton::clicked,
            this, &VideoPlayer::on_mute_clicked);

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setRange(0, 0);
    m_positionSlider->setMinimumWidth(200);

    connect(m_positionSlider, &QAbstractSlider::sliderMoved,
            this, &VideoPlayer::setPosition);

    m_fileName = new QLabel;
    m_fileName->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    m_fileName->setText("Press the File Icon to open a media file.");

    durat = new QLabel;
    ndurat = new QLabel;
    durat->setText("00:00");
    ndurat->setText("00:00");
    plButton = new QPushButton;
//    plButton->setText(tr("Show Playlist"));
    plButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarShadeButton));
    plButton->setCheckable(true);

    QSpacerItem *spacer = new QSpacerItem(100, 10, QSizePolicy::Expanding, QSizePolicy::Minimum);

    sesSlider = new QSlider;
    sesSlider->setValue(50);
    sesSlider->setMinimumWidth(100);
    m_mediaPlayer->setVolume(50);
    sesSlider->setOrientation(Qt::Horizontal);

    connect(sesSlider, &QAbstractSlider::sliderMoved,
            this, &VideoPlayer::on_sesSlider_valueChanged);

    QBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->setMargin(0);
    controlLayout->addWidget(open);
    controlLayout->addWidget(m_playButton);
    controlLayout->addWidget(ndurat);
    controlLayout->addWidget(m_positionSlider);
    controlLayout->addWidget(durat);
    controlLayout->addWidget(exitButton);
//    controlLayout->setSpacing(0);

    QHBoxLayout *sesControl = new QHBoxLayout;
    sesControl->addWidget(m_fileName);
    sesControl->addItem(spacer);
    sesControl->addWidget(plButton);
    sesControl->addWidget(mute);
    sesControl->addWidget(sesSlider);
//    sesControl->setSpacing(0);

//    11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111

    plbaglac = new QFrame;
    plbaglac->setGeometry(QRect(12, 12, 380, 102));
    plbaglac->setMaximumSize(QSize(16777215, 102));
    plnames = new QListWidget(plbaglac);
    plnames->setObjectName(QStringLiteral("pl"));
    plnames->setMaximumSize(QSize(16777215, 107));

    horizontalLayout = new QHBoxLayout(plbaglac);
    horizontalLayout->addWidget(plnames);
//    horizontalLayout->setSpacing(0);

    addtopl = new QPushButton(plbaglac);
    addtopl->setText(tr("Add..."));
    playnow = new QPushButton(plbaglac);
    playnow->setText(tr("Play Now"));
    deletefrompl = new QPushButton(plbaglac);
    deletefrompl->setText("Delete");
//    deletefrompl->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));

    verticalLayout = new QVBoxLayout();
    verticalLayout->addWidget(addtopl);
    verticalLayout->addWidget(playnow);
    verticalLayout->addWidget(deletefrompl);
//    verticalLayout->setSpacing(0);

    horizontalLayout->addLayout(verticalLayout);


    plbaglac->hide();

//    connect(plButton, &QAbstractButton::toggled, plbaglac, &QWidget::setVisible);
    connect(plButton, &QAbstractButton::toggled,
            this, &VideoPlayer::playlistclicked);

    connect(plnames, &QListWidget::doubleClicked,
            this, &VideoPlayer::playnow_clicked);


//    11111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111


    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(vWidget);
    layout->addLayout(controlLayout);
    layout->addLayout(sesControl);
    layout->addWidget(plbaglac);
//    layout->setSpacing(0);
//    setLayout(layout);

    QWidget *mainw = new QWidget;
    mainw->setLayout(layout);
    vWidget->setStyleSheet("background-color:black");
//    mainw->setStyleSheet("background-color:#8FFF8B");

    QBoxLayout *mainl = new QHBoxLayout;
    mainl->addWidget(mainw);
//    mainl->setSpacing(0);
    setLayout(mainl);

    m_mediaPlayer->setVideoOutput(vWidget);
    connect(m_mediaPlayer, &QMediaPlayer::stateChanged,
            this, &VideoPlayer::sChanged);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged,
            this, &VideoPlayer::positionChanged);
    connect(m_mediaPlayer, &QMediaPlayer::durationChanged,
            this, &VideoPlayer::durationChanged);
    connect(addtopl, &QAbstractButton::clicked,
            this, &VideoPlayer::add_clicked);
    connect(deletefrompl, &QAbstractButton::clicked,
            this, &VideoPlayer::delete_clicked);
    connect(playnow, &QAbstractButton::clicked,
            this, &VideoPlayer::playnow_clicked);


    playlist = new QMediaPlaylist;

    m_mediaPlayer->setPlaylist(playlist);



    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateVal()));
    timer->start();


}

void VideoPlayer::updateVal()
{
    if (durat->text()==ndurat->text())
        {
            if (durat->text()!="00:00")
            {
                plnames->setCurrentRow(0);
                if(plnames->count()!=1)
                {
                    plnames->model()->removeRow(plnames->currentRow());
                    plnames->setCurrentRow(0);
                    playnow_clicked();
                }
            }
        }
    if (plnames->count()==0)
    {
        deletefrompl->setDisabled(1);
        playnow->setDisabled(1);
    }else {
        deletefrompl->setDisabled(0);
        playnow->setDisabled(0);
    }
}



VideoPlayer::~VideoPlayer()
{
}

void VideoPlayer::playlistclicked()
{
    if(plbaglac->isVisible())
    {
        plbaglac->setVisible(0);
//        plButton->setText("Show Playlist");
        plButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarShadeButton));
    } else {
        plbaglac->setVisible(1);
//        plButton->setText("Hide Playlist");
        plButton->setIcon(style()->standardIcon(QStyle::SP_TitleBarUnshadeButton));
    }
}


void VideoPlayer::open()
{
    QFileDialog fileDialog(this);
    fileDialog.setWindowTitle(tr("Open a Media File"));
    QUrl filedir = fileDialog.getOpenFileUrl();
    QString filenmst = filedir.toString();
    if(filenmst.contains("file:///"))
    {
        filenmst.replace("file:///","");
    }
    if(filenmst.contains(".mp4")||filenmst.contains(".avi")||filenmst.contains(".mpg")
            ||filenmst.contains(".wmv")||filenmst.contains(".wma")||filenmst.contains(".wav")
            ||filenmst.contains(".asf")||filenmst.contains(".m4v")||filenmst.contains(".mov")
            ||filenmst.contains(".mpeg")||filenmst.contains(".mp3")||filenmst.contains(".mkv"))
    {
        m_fileName->setText(filenmst);
        plnames->addItem(filenmst);
        setUrl(filedir);
    }else {
        m_fileName->setText("File Type is not supported.");
    }
    play();
}

void VideoPlayer::setUrl(const QUrl &url)
{
    setWindowFilePath(url.isLocalFile() ? url.toLocalFile() : QString());
    m_mediaPlayer->setMedia(url);
    m_playButton->setEnabled(true);
}

void VideoPlayer::play()
{
    switch (m_mediaPlayer->state()) {
    case QMediaPlayer::PlayingState:
        m_mediaPlayer->pause();
        break;
    default:
        m_mediaPlayer->play();
        break;
    }
}

void VideoPlayer::sChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
//        m_playButton->setText(tr("Pause"));
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
//        m_playButton->setText(tr("Play"));
        m_playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}



static QString formatTime(qint64 timeMilliSeconds)
{
    qint64 seconds = timeMilliSeconds / 1000;
    const qint64 minutes = seconds / 60;
    seconds -= minutes * 60;
    return QStringLiteral("%1:%2")
        .arg(minutes, 2, 10, QLatin1Char('0'))
        .arg(seconds, 2, 10, QLatin1Char('0'));
}



void VideoPlayer::positionChanged(qint64 position)
{
    m_positionSlider->setValue(position);
    ndurat->setText(formatTime(m_mediaPlayer->position()));
}

void VideoPlayer::durationChanged(qint64 duration)
{
    m_positionSlider->setRange(0, duration);

    QTime timeval(0,0,0,0);
    timeval = timeval.addMSecs(duration);
    durat->setText(timeval.toString("mm:ss"));
}

void VideoPlayer::setPosition(int position)
{
    m_mediaPlayer->setPosition(position);
}

void VideoPlayer::on_mute_clicked()
{
    if (m_mediaPlayer->isMuted()) {
        m_mediaPlayer->setMuted(false);
        mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    } else {
        m_mediaPlayer->setMuted(true);
        mute->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
    }
}

void VideoPlayer::on_sesSlider_valueChanged(int value)
{
    m_mediaPlayer->setVolume(value);
}

void VideoPlayer::add_clicked()
{

    QFileDialog addfileDialog(this);
    addfileDialog.setWindowTitle(tr("Add a Media File"));
    QUrl addedfiledir = addfileDialog.getOpenFileUrl();
    QString addedfilenamest = addedfiledir.toString();
    if(addedfilenamest.contains("file:///"))
    {
        addedfilenamest.replace("file:///","");
    }


    if(addedfilenamest.contains(".mp4")||addedfilenamest.contains(".avi")||addedfilenamest.contains(".mpg")
            ||addedfilenamest.contains(".wmv")||addedfilenamest.contains(".wma")||addedfilenamest.contains(".wav")
            ||addedfilenamest.contains(".asf")||addedfilenamest.contains(".m4v")||addedfilenamest.contains(".mov")
            ||addedfilenamest.contains(".mpeg")||addedfilenamest.contains(".mp3")||addedfilenamest.contains(".mkv"))
    {
        plnames->addItem(addedfilenamest);
        playlist->addMedia(addedfiledir);
    }else {
        m_fileName->setText("File Type of the file you just added is not supported. File will not be added to the Playlist.");
    }



//    setUrl(addedfiledir);


}

void VideoPlayer::playnow_clicked()
{
    if(plnames->count()!=0)
    {
    if(plnames->currentItem()->text().contains(".mp4")||plnames->currentItem()->text().contains(".avi")
            ||plnames->currentItem()->text().contains(".mpg")||plnames->currentItem()->text().contains(".wmv")
            ||plnames->currentItem()->text().contains(".wma")||plnames->currentItem()->text().contains(".wav")
            ||plnames->currentItem()->text().contains(".asf")||plnames->currentItem()->text().contains(".m4v")
            ||plnames->currentItem()->text().contains(".mov")||plnames->currentItem()->text().contains(".mpeg")
            ||plnames->currentItem()->text().contains(".mp3")||plnames->currentItem()->text().contains(".mkv"))
    {
        m_fileName->setText(plnames->currentItem()->text());
        setUrl("file:///" + plnames->currentItem()->text());
    }else {
        m_fileName->setText("File Type is not supported.");
    }

    plnames->addItem(plnames->currentItem()->text());
    plnames->model()->removeRow(plnames->currentRow());

    for (int i = 1; i < plnames->count(); i++)
    {
        plnames->setCurrentRow(0);
        plnames->addItem(plnames->currentItem()->text());
        plnames->model()->removeRow(plnames->currentRow());
    }
        m_mediaPlayer->play();
    }
}

void VideoPlayer::delete_clicked()
{
    plnames->model()->removeRow(plnames->currentRow());
}
//QString VideoPlayer::whatname(const QString& nameis)
//{
//    nameis = m_fileName->text();
//    return nameis;
//}
