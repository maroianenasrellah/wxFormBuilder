rm -r ../../wxfb
svn export ../bin ../../wxfb
cp ../bin/wxFormBuilder ../../wxfb
cp -R ../bin/lib ../../wxfb
rm ../../wxfb/lib/*d.so
cp ../bin/plugins/additional/libadditional.so ../../wxfb/plugins/additional
cp ../bin/plugins/common/libcommon.so ../../wxfb/plugins/common
cp ../bin/plugins/layout/liblayout.so ../../wxfb/plugins/layout
cp ../bin/plugins/wxAdditions/libwxadditions\-mini.so ../../wxfb/plugins/wxAdditions
cp /opt/wx/2.8.2/lib/libwx_gtk2u-2.8.so.0.1.0 ../../wxfb/lib/libwx_gtk2u-2.8.so.0 

