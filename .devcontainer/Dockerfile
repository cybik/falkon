FROM invent-registry.kde.org/neon/docker-images/all:latest

USER root

# BROKEN: KDE Neon images' dependency graph is in a bad state
RUN apt install -y  qt6-base-dev qt6-webengine-dev qt6-5compat-dev qt6-declarative-dev

# BROKEN: KDE Neon images have KDE5 preinstalled and there's a huge clash
RUN apt install -y  kf6-karchive-dev kf6-kwallet-dev kf6-kcrash-dev  \
                    kf6-kcoreaddons-dev kf6-purpose-dev kf6-kjobwidgets-dev
