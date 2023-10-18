package com.lyhao.xvideoplayer.media;

public class FFMediaPlayer {
    static {
        System.loadLibrary("ffmpeg-lib");
    }


    public static String getFFmpegVersion() {
        return nativeGetFFmpegVersion();
    }
    private static native String nativeGetFFmpegVersion();
}
