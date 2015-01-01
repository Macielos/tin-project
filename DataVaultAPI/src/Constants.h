/**
 *  Działanie zlecane serwerowi przez klienta
 */
enum Action {REGISTER, LOGIN, LOGOUT, UNREGISTER, LIST, UPLOAD, DOWNLOAD, REMOVE, RENAME, GIVE_ACCESS, REVOKE_ACCESS};

/**
 *  Tyle razy metoda sendNotification() będzie próbowac połączyć się z drugą, aby wysłąć jej powiadomienie,
 *  że można przesyłać kolejny plik
 */
const unsigned int NOTIFICATION_RETRIES = 10;

/**
 *  Przerwa w milisekundach między kolejnymi próbami nawiązania połączenia przez metodę sendNotification()
 */
const unsigned int RETRY_INTERVAL = 200;
