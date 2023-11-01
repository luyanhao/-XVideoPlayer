package com.lyhao.xvideoplayer.media;

import android.view.Surface;

public class FFMediaPlayer {
    static {
        System.loadLibrary("ffmpeg-lib");
    }
    //player type
    public static final int FFMEDIA_PLAYER = 0;
    public static final int HWCODEC_PLAYER = 1;

    //render type
    public static final int VIDEO_RENDER_OPENGL         = 0;
    public static final int VIDEO_RENDER_ANWINDOW       = 1;
    public static final int VIDEO_RENDER_3D_VR          = 2;

    private long mNativePlayerHandle = 0;

    public static String getFFmpegVersion() {
        return nativeGetFFmpegVersion();
    }
    private static native String nativeGetFFmpegVersion();

    public void init(String url, int videoRenderType, Surface surface) {
        mNativePlayerHandle = nativeInit(url, FFMEDIA_PLAYER, videoRenderType, surface);
    }

    public void play() {
        nativePlay(mNativePlayerHandle);
    }

    private native long nativeInit(String url, int playerType, int renderType, Object surface);
    private native long nativePlay(long playerHandle);
}
