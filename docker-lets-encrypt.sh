#!/bin/bash
set -e

# see https://www.humankode.com/ssl/how-to-set-up-free-ssl-certificates-from-lets-encrypt-using-docker-and-nginx

if [ $1 == "--staging" ] || [ $1 == "--production" ]; then
  MODE=$1
else
  echo "Invalid arguments supplied, first argument must be --staging or --production"
  exit 1
fi

if [ $1 = "--production" ]; then
  if [ $DROPBOX_TOKEN == "" ]; then
    echo "Missing DROPBOX_TOKEN environment variable"
    exit 3
  fi

  echo "Running in production mode."

  # Backup if we have actual certificates
  if [ "$(ls -A /root/app/certs/etc/letsencrypt)" ]; then
    # Remove previous zip if it exists
    if [ -f "certs.zip" ]; then
      echo "Deleting previous certificates backup"
      rm certs.zip
    fi

    # Zip certificates
    echo "Backing up current certificates"
    zip -r certs.zip /root/app/certs

    # Upload to Dropbox
    echo "Uploading certificate backup to Dropbox"
    curl -X POST https://content.dropboxapi.com/2/files/upload \
      --header "Authorization: Bearer $DROPBOX_TOKEN" \
      --header "Dropbox-API-Arg: {\"path\": \"/certs.zip\",\"mode\": \"overwrite\",\"autorename\": true,\"mute\": false,\"strict_conflict\": false}" \
      --header "Content-Type: application/octet-stream" \
      --data-binary @certs.zip
  fi

  if [ "$(ls -A /root/app/certs)" ]; then
    echo "Deleting existing /root/app/certs"
    rm -rf /root/app/certs

    echo "Creating empty /root/app/certs"
    mkdir /root/app/certs
  fi

  echo "Generating new certificates"
  sudo docker run -it --rm \
    -v /root/app/certs/etc/letsencrypt:/etc/letsencrypt \
    -v /root/app/certs/var/lib/letsencrypt:/var/lib/letsencrypt \
    -v /root/app/certs/var/log/letsencrypt:/var/log/letsencrypt \
    -v /root/app/public:/app/public \
    certbot/certbot \
      certonly --webroot \
      --email daniels.bytes@gmail.com \
      --no-eff-email \
      --agree-tos \
      --webroot-path=/app/public \
      -d daniel-battaglia.com \
      -d www.daniel-battaglia.com \
      -d resume.daniel-battaglia.com \
      -d danielbytes.dev \
      -d www.danielbytes.dev \
      -d resume.danielbytes.dev

  # This must happen after generating the new certificate
  echo "Stopping application container"
  docker stop resume

  # Overwrite file on server
  echo "Overwriting server certificates to /root/app/certs"
  cp -f /root/app/certs/etc/letsencrypt/live/daniel-battaglia.com/fullchain.pem /usr/local/share/ca-certificates/fullchain.pem

  echo "Starting application container"
  docker start resume
else
  echo "Running in staging mode.  To run in production mode, add the --production command line argument."
  
  sudo docker run -it --rm \
    -v /root/app/certs/etc/letsencrypt:/etc/letsencrypt \
    -v /root/app/certs/var/lib/letsencrypt:/var/lib/letsencrypt \
    -v /root/app/certs/var/log/letsencrypt:/var/log/letsencrypt \
    -v /root/app/public:/app/public \
    certbot/certbot \
      certonly --webroot \
      --register-unsafely-without-email \
      --agree-tos \
      --webroot-path=/app/public \
      --staging \
      -d daniel-battaglia.com \
      -d www.daniel-battaglia.com \
      -d resume.daniel-battaglia.com \
      -d danielbytes.dev \
      -d www.danielbytes.dev \
      -d resume.danielbytes.dev
fi
