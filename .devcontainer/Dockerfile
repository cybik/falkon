FROM fedora:rawhide

RUN dnf check-update
RUN dnf install -y zsh util-linux git

RUN sh -c "$(curl -fsSL https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" --unattended

ENTRYPOINT [ "/bin/zsh" ]

RUN dnf -y install  extra-cmake-modules libxcb-devel xcb-util-devel openssl-devel
RUN dnf -y install  qt6-qtbase-devel qt6-qtwebengine-devel \
                    qt6-qt5compat-devel qt6-qtdeclarative-devel
RUN dnf -y install  kf6-kcrash-devel kf6-kio-devel \
                    kf6-kjobwidgets-devel kf6-karchive-devel \
                    kf6-kwallet-devel kf6-purpose-devel \
                    kf6-ki18n-devel