package com.lyhao.xvideoplayer.util;

import android.util.Log;

/**
 * Created by lyhao on 2023/10/30.
 */
public class LogUtil {
    private static String TAG = "XV_";
    public static void d(String sub, String msg) {
        Log.d(TAG + sub, msg);
    }
}
