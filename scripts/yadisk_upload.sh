FOLDER_NAME=$DEPLOY_BRANCH-$TRAVIS_BRANCH
WEBDAV_SRV=https://$YANDEX_DISK_USER:$YANDEX_DISK_TOKEN@webdav.yandex.ru

for file in $*; do
	echo "Uploading $file..."
	curl -T $file $WEBDAV_SRV/$FOLDER_NAME/$file
done
