package com.lyhao.xvideoplayer.util;

import android.content.Context;
import android.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;

public class CommonUtils {
    private static final String TAG = "CommonUtils";

    public static void copyAssetsDirToSDCard(Context context, String assetsDirName, String sdCardPath) {
        Log.d(TAG, "copyAssetsDirToSDCard() called with: context = [" + context + "], assetsDirName = [" + assetsDirName + "], sdCardPath = [" + sdCardPath + "]");
        try {
            String list[] = context.getAssets().list(assetsDirName);
            LogUtil.d(TAG, "list=====" + list.length);
            if (list.length == 0) {
                InputStream inputStream = context.getAssets().open(assetsDirName);
                byte[] mByte = new byte[1024];
                int bt = 0;
                File file = new File(sdCardPath + File.separator
                        + assetsDirName.substring(assetsDirName.lastIndexOf('/')));
                if (!file.exists()) {
                    LogUtil.d(TAG, "file not exists~~~");
                    file.createNewFile();
                } else {
                    LogUtil.d(TAG, "file exists return~~~");
                    return;
                }
                FileOutputStream fos = new FileOutputStream(file);
                while ((bt = inputStream.read(mByte)) != -1) {
                    fos.write(mByte, 0, bt);
                }
                fos.flush();
                inputStream.close();
                fos.close();
            } else {
                String subDirName = assetsDirName;
                if (assetsDirName.contains("/")) {
                    subDirName = assetsDirName.substring(assetsDirName.lastIndexOf('/') + 1);
                }
                sdCardPath = sdCardPath + File.separator + subDirName;
                File file = new File(sdCardPath);
                if (!file.exists())
                    file.mkdirs();
                for (String s : list) {
                    LogUtil.d(TAG, "-=-=-=-=-=-=-=-=" + s);
                    copyAssetsDirToSDCard(context, assetsDirName + File.separator + s, sdCardPath);
                }
            }
        } catch (
                Exception e) {
            e.printStackTrace();
        }
    }
}
