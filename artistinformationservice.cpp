#include "artistinformationservice.h"
#include "kodiclient.h"
#include "utils.h"
#include "albumsrequest.h"

namespace {
int filesPropCount(QQmlListProperty<KodiFile>* list)
{
    return static_cast<QList<KodiFile*>*>(list->data)->count();
}

KodiFile* filesPropAt(QQmlListProperty<KodiFile>* list, int index)
{
    return static_cast<QList<KodiFile*>*>(list->data)->at(index);
}
}

int ArtistInformationService::artistId() const
{
    return artistId_;
}

void ArtistInformationService::setArtistId(int artistId)
{
    artistId_ = artistId;
    emit artistIdChanged();
}

QString ArtistInformationService::artistName() const
{
    return artistName_;
}

void ArtistInformationService::setArtistName(const QString &artistName)
{
    artistName_ = artistName;
    emit artistNameChanged();
}

QString ArtistInformationService::artistDescription() const
{
    return artistDescription_;
}

void ArtistInformationService::setArtistDescription(const QString &artistDescription)
{
    artistDescription_ = artistDescription;
    emit artistDescriptionChanged();
}

QString ArtistInformationService::fanart() const
{
    return fanart_;
}

void ArtistInformationService::setFanart(const QString &fanart)
{
    fanart_ = fanart;
    emit fanartChanged();
}

QString ArtistInformationService::thumbnail() const
{
    return thumbnail_;
}

void ArtistInformationService::setThumbnail(const QString &thumbnail)
{
    thumbnail_ = thumbnail;
    emit thumbnailChanged();
}

QString ArtistInformationService::genres() const
{
    if(genres_.size() == 0)
        return QString("");
    QString ret = genres_[0];
    for(int i = 1; i < genres_.size(); ++i)
    {
        ret = "," + genres_[i];
    }
    return ret;
}

QString ArtistInformationService::style() const
{
    if(style_.size() == 0)
        return QString("");
    QString ret = style_[0];
    for(int i = 1; i < style_.size(); ++i)
    {
        ret = "," + style_[i];
    }
    return ret;
}

QQmlListProperty<KodiFile> ArtistInformationService::albums()
{
    return QQmlListProperty<KodiFile>(this, &albums_, &filesPropCount, &filesPropAt);
}

ArtistInformationService::ArtistInformationService(QObject *parent) : QObject(parent)
{

}

void ArtistInformationService::refresh()
{
    QJsonObject parameters;
    parameters["artistid"] = artistId_;
    QJsonArray properties;
    properties.append(QString("style"));
    properties.append(QString("description"));
    properties.append(QString("genre"));
    properties.append(QString("fanart"));
    properties.append(QString("thumbnail"));
    parameters["properties"] = properties;
    QJsonRpcMessage message = QJsonRpcMessage::createRequest("AudioLibrary.GetArtistDetails", parameters);
    auto reply = KodiClient::current().send(message);
    connect(reply, &QJsonRpcServiceReply::finished, this, &ArtistInformationService::handleRefresh_);
    auto albumsQuery = new AlbumsRequest();
    connect(albumsQuery, &AlbumsRequest::finished, this, &ArtistInformationService::handleAlbums_);
    albumsQuery->start(artistId_);
}

void ArtistInformationService::handleRefresh_()
{
    auto reply = dynamic_cast<QJsonRpcServiceReply*>(sender());
    if(reply)
    {
        auto result = reply->response().result();
        if(!result.isObject())
            return;
        auto detailsTmp = result.toObject().value("artistdetails");
        if(!detailsTmp.isObject())
            return;
        auto details = detailsTmp.toObject();
        setArtistName(details.value("label").toString());
        setArtistDescription(details.value("description").toString());
        auto genreTmp = details.value("genre");
        if(genreTmp.isArray())
        {
            auto genre = genreTmp.toArray();
            genres_.clear();
            for(auto const& value : genre)
            {
                if(value.isString())
                    genres_.push_back(value.toString());
            }
            emit genresChanged();
        }
        auto styleTmp = details.value("style");
        if(styleTmp.isArray())
        {
            auto style = styleTmp.toArray();
            style_.clear();
            for(auto const& value : style)
            {
                if(value.isString())
                    style_.push_back(value.toString());
            }
            emit styleChanged();
        }
        QByteArray b = details.value("fanart").toString().toUtf8();
        setFanart(QString::fromLatin1(b.toBase64()));
        b = details.value("thumbnail").toString().toUtf8();
        setThumbnail(QString::fromLatin1(b.toBase64()));
    }
}

void ArtistInformationService::handleAlbums_()
{
    auto albumsQuery = dynamic_cast<AlbumsRequest*>(sender());
    if(albumsQuery)
    {
        albums_ = std::move(albumsQuery->results);
        emit albumsChanged();
        albumsQuery->deleteLater();
    }
}
