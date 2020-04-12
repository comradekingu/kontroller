#ifndef EU_TGCM_KONTROLLER_APPLICATIONSETTINGS_H
#define EU_TGCM_KONTROLLER_APPLICATIONSETTINGS_H

#include "server.h"
#include "downloadlocation.h"

#include <QQmlListProperty>
#include <QString>
#include <QVector>

namespace eu
{
namespace tgcm
{
namespace kontroller
{

enum class DeviceType {
	Undefined,
	Phone,
	Tablet,
	Laptop,
	Desktop,
	TV
};

/**
 * @brief The ApplicationSettings class is a class used to store and update
 * application parameters. It should not be instanciated more than once in
 * the application, although nothing enforce this
 */
class ApplicationSettings : public QObject
{
	Q_OBJECT
private:
	QVector<Server*> servers_;
	/*    QString serverAddress_;
	int serverPort_;
	bool musicFileBrowsing_;
	bool videosFileBrowsing_;
	bool useHttpInterface_;
	DeviceType deviceType_; */
#ifndef SAILFISH_TARGET
	int dpi_;
#endif
	/*    int serverHttpPort_; */
	bool ignoreWifiStatus_;
	QString downloadFolder_;
	QString lastServer_;

	QVector<DownloadLocation*> possibleDownloadLocations_;

	static int getServerCount_(QQmlListProperty<Server>* list);
	static Server* getServerAt_(QQmlListProperty<Server>* list, int index);

	static int getDownloadLocationCount_(QQmlListProperty<DownloadLocation>* list);
	static DownloadLocation* getDownloadLocationAt_(QQmlListProperty<DownloadLocation>* list, int index);
public:
	explicit ApplicationSettings(QObject* parent = nullptr);

	Q_PROPERTY(bool ignoreWifiStatus READ ignoreWifiStatus WRITE setIgnoreWifiStatus NOTIFY ignoreWifiStatusChanged)
	Q_PROPERTY(QString downloadFolder READ downloadFolder WRITE setDownloadFolder NOTIFY downloadFolderChanged)
	Q_PROPERTY(QString lastServer READ lastServer WRITE setLastServer NOTIFY lastServerChanged)
	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::Server> servers READ servers NOTIFY serversChanged)
	Q_PROPERTY(QQmlListProperty<eu::tgcm::kontroller::DownloadLocation> possibleDownloadLocations READ \
	           possibleDownloadLocations NOTIFY possibleDownloadLocationsChanged)

	QQmlListProperty<Server> servers();

	/*    bool useHttpInterface() const;
	void setUseHttpInterface(bool useHttpInterface);

	DeviceType deviceType() const;
	void setDeviceType(DeviceType type); */

#ifndef SAILFISH_TARGET
	int dpi() const;
	void setDpi(int dpi);
#endif
	bool ignoreWifiStatus() const;
	void setIgnoreWifiStatus(bool ignoreWifiStatus);
	QString downloadFolder() const;
	void setDownloadFolder(const QString &downloadFolder);

	void setLastServer(QString lastServerUuid);

	QString lastServer() const;

	int lastServerIndex() const;
	QQmlListProperty<DownloadLocation> possibleDownloadLocations();
signals:
	void lastServerChanged();
	void serversChanged();
	void currentServerChanged();
	void ignoreWifiStatusChanged(bool ignoreWifiStatus);
#ifndef SAILFISH_TARGET
	void dpiChanged(int dpi);
#endif

	void downloadFolderChanged(QString downloadFolder);
	void possibleDownloadLocationsChanged();

public slots:
	void save();
	eu::tgcm::kontroller::Server* server(QString const& uuid);
	eu::tgcm::kontroller::Server* newServer();
	bool deleteServer(QString const& uuid);
	void setDownloadLocation(DownloadLocation* downloadLocation);
};

}
}
}

#endif // EU_TGCM_KONTROLLER_APPLICATIONSETTINGS_H
