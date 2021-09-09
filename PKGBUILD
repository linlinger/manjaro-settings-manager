# Maintainer : linlinger <linyx@stp.net.cn>
# Contributor : Ramon Buldo <ramon@manjaro.org>

pkgbase=manjaro-settings-manager
pkgname=('manjaro-settings-manager' 'manjaro-settings-manager-kcm' 
         'manjaro-settings-manager-notifier' 'manjaro-settings-manager-knotifier')
pkgver=0.5.6
#_commit=e085b661c3c97e8aa39a5b49896a88a95d84caf9
pkgrel=1
pkgdesc="Linux System Settings Tool developed by Manjaro Linux"
arch=('i686' 'x86_64')
url="https://github.com/linlinger/manjaro-settings-manager"
license=("GPL")
depends=('icu' 'qt5-base' 'hwinfo' 'kitemmodels' 'kauth' 
         'kcoreaddons' 'ckbcomp' 'xdg-utils')
optdepends=('manjaro-settings-manager-notifier: qt-based'
            'manjaro-settings-manager-knotifier: knotifications-based')
makedepends=('extra-cmake-modules' 'kdoctools' 'qt5-tools' 'knotifications' 
             'kconfigwidgets' 'kcmutils')
conflicts=('kcm-msm')
# source=("msm-$pkgver-$pkgrel.tar.gz::$url/-/archive/$_commit/$pkgname-$_commit.tar.gz")
# source=("msm-$pkgver-$pkgrel.tar.gz::$url/-/archive/$pkgver/$pkgbase-$pkgver.tar.gz")
source=("git+https://github.com/linlinger/${pkgname}.git")
sha256sums=('SKIP')

prepare() {
#  mv ${pkgbase}-${_commit} ${pkgbase}-${pkgver}
  cd "$srcdir/${pkgbase}-${pkgver}"
  # patches here
}

build() {
  cd "$srcdir/${pkgbase}-${pkgver}"
  mkdir -p build
  cd build
  cmake ../ \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DLIB_INSTALL_DIR=lib \
    -DKDE_INSTALL_USE_QT_SYS_PATHS=ON \
    -DSYSCONF_INSTALL_DIR=/etc
  CXXFLAGS+="-std=gnu++98" make
}

package_manjaro-settings-manager() {
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install 
  rm -rf $pkgdir/usr/bin/msm_notifier
  rm -rf $pkgdir/usr/bin/msm_kde_notifier
  rm -rf $pkgdir/usr/lib/qt
  rm -rf $pkgdir/usr/share/kservices5
  rm -rf $pkgdir/usr/share/applications/msm_notifier_settings.desktop
  rm -rf $pkgdir/usr/share/applications/msm_kde_notifier_settings.desktop
  rm -rf $pkgdir/etc/xdg
}

package_manjaro-settings-manager-kcm() {
  pkgdesc="Manjaro Linux System Settings Tool (KCM for Plasma 5)"
  depends=('manjaro-settings-manager' 'kcmutils' 'kconfigwidgets')
  replaces=('kcm-msm')
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc  
  rm -rf $pkgdir/usr/bin
  rm -rf $pkgdir/usr/lib/kauth
  rm -rf $pkgdir/usr/share/{applications,dbus-1,icons,polkit-1}
}

package_manjaro-settings-manager-notifier() {
  pkgdesc="Manjaro Linux System Settings Tool (Notifier)"
  depends=('manjaro-settings-manager')
  provides=('manjaro-settings-manager-kde-notifier')
  conflicts=('manjaro-settings-manager-kde-notifier')
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc/dbus-1
  rm -rf $pkgdir/etc/xdg/autostart/msm_kde_notifier.desktop
  rm -rf $pkgdir/usr/lib/
  rm -rf $pkgdir/usr/share/{kservices5,dbus-1,icons,polkit-1}
  rm -rf $pkgdir/usr/share/applications/manjaro*
  rm -rf $pkgdir/usr/share/applications/msm_kde_notifier_settings.desktop
  rm -rf $pkgdir/usr/bin/manjaro*
  rm -rf $pkgdir/usr/bin/msm_kde_notifier
}

package_manjaro-settings-manager-knotifier() {
  pkgdesc="Manjaro Linux System Settings Tool (Notifier for Plasma 5)"
  depends=('manjaro-settings-manager' 'knotifications')
  conflicts=('manjaro-settings-manager-notifier')
  replaces=('manjaro-settings-manager-kde-notifier')
  cd "$srcdir/${pkgbase}-${pkgver}/build"
  make DESTDIR=${pkgdir} install
  rm -rf $pkgdir/etc/dbus-1
  rm -rf $pkgdir/etc/xdg/autostart/msm_notifier.desktop
  rm -rf $pkgdir/usr/lib/
  rm -rf $pkgdir/usr/share/{kservices5,dbus-1,icons,polkit-1}
  rm -rf $pkgdir/usr/share/applications/manjaro*
  rm -rf $pkgdir/usr/share/applications/msm_notifier_settings.desktop
  rm -rf $pkgdir/usr/bin/manjaro*
  rm -rf $pkgdir/usr/bin/msm_notifier
} 
