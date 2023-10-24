package com.lyhao.xvideoplayer.media;

import android.view.Surface;

public class FFMediaPlayer {
    static {
        System.loadLibrary("ffmpeg-lib");
    }


    public static String getFFmpegVersion() {
        return nativeGetFFmpegVersion();
    }
    private static native String nativeGetFFmpegVersion();

    public void init(String url, int videoRenderType, Surface surface) {

    }
}
