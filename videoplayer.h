#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QWidget>
#include <QSpacerItem>
#include <QBoxLayout>
#include <QListWidget>
#include <QFrame>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QSlider;
class QLabel;
class QUrl;
QT_END_NAMESPACE

class VideoPlayer : public QWidget
{
    Q_OBJECT
public:
    VideoPlayer(QWidget *parent = nullptr);
    ~VideoPlayer();

    void setUrl(const QUrl &url);

public slots:
    void open();
    void play();
    void on_mute_clicked();

private slots:
    void sChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void setPosition(int position);
    void on_sesSlider_valueChanged(int value);
    void add_clicked();
    void playnow_clicked();
    void delete_clicked();
    void updateVal();
    void playlistclicked();

private:
    QMediaPlayer* m_mediaPlayer;
    QMediaPlaylist* playlist;
    QAbstractButton *m_playButton;
    QAbstractButton *exitButton;
    QAbstractButton *plButton;
    QSlider *m_positionSlider;
    QLabel *m_fileName;
    QLabel *durat;
    QLabel *ndurat;
    QSlider *sesSlider;
    QAbstractButton *mute;
    QFrame *plbaglac;


//    Playlist Components

public:
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout;
    QListWidget *plnames;
    QAbstractButton *addtopl;
    QAbstractButton *playnow;
    QAbstractButton *deletefrompl;
    QSpacerItem *verticalSpacer;

//    Playlist Components
};

#endif
