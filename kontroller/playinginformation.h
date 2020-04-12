#ifndef EU_TGCM_KONTROLLER_PLAYINGINFORMATION_H
#define EU_TGCM_KONTROLLER_PLAYINGINFORMATION_H

#include <QObject>

#include "playerservice.h"
#include "playlistservice.h"

namespace eu
{
namespace tgcm
{
namespace kontroller
{

class PlayingInformation : public QObject
{
	Q_OBJECT
	QString playerType_;
	QString mediaTitle_;
	QString mediaId_;
	int artistId_;
	PlaylistItem* item_ = nullptr;

public:
	explicit PlayingInformation(QObject *parent = 0);
	Q_PROPERTY(eu::tgcm::kontroller::Client* client READ client WRITE setClient NOTIFY clientChanged)
	Q_PROPERTY(QString playerType READ playerType WRITE setPlayerType NOTIFY playerTypeChanged)
	Q_PROPERTY(QString mediaTitle READ mediaTitle WRITE setMediaTitle NOTIFY MediaTitleChanged)
	Q_PROPERTY(QString mediaId READ mediaId WRITE setMediaId NOTIFY mediaIdChanged)
	Q_PROPERTY(int artistId READ artistId WRITE setArtistId NOTIFY artistIdChanged)
	Q_PROPERTY(eu::tgcm::kontroller::PlaylistItem* currentItem READ currentItem NOTIFY currentItemChanged)
	Q_PROPERTY(eu::tgcm::kontroller::PlaylistItem* nextItem READ nextItem NOTIFY currentItemChanged)

	QString playerType() const;

	QString mediaTitle() const;

	QString mediaId() const;

	int artistId() const;

	PlaylistItem* currentItem();
	PlaylistItem* nextItem();

	eu::tgcm::kontroller::PlaylistService* playlistService() const;

	eu::tgcm::kontroller::Client* client() const;

private:
	void refreshCurrentlyPlaying_(int playerid);

	eu::tgcm::kontroller::PlaylistService* playlistService_ = nullptr;

	eu::tgcm::kontroller::Client* client_ = nullptr;

signals:

	void playerTypeChanged(QString playerType);

	void MediaTitleChanged(QString mediaTitle);

	void mediaIdChanged(QString mediaId);

	void artistIdChanged(int artistId);

	void currentItemChanged();

	void clientChanged(eu::tgcm::kontroller::Client* client);

public slots:
	void setPlayerType(QString playerType);
	void setMediaTitle(QString mediaTitle);
	void setMediaId(QString mediaId);
	void setArtistId(int artistId);

	void setClient(eu::tgcm::kontroller::Client* client);

private slots:
	void refreshCurrentPlayer_();
	void handleGetItemResponse_();
};

}
}
}

#endif // EU_TGCM_KONTROLLER_PLAYINGINFORMATION_H
