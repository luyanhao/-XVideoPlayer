package com.lyhao.xvideoplayer.media;

import android.view.Surface;

import com.lyhao.xvideoplayer.util.LogUtil;

import java.util.Locale;

public class FFMediaPlayer {
    public static final String TAG = "FFMediaPlayer";
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

    public static final int MSG_DECODER_INIT_ERROR      = 0;
    public static final int MSG_DECODER_READY           = 1;
    public static final int MSG_DECODER_DONE            = 2;
    public static final int MSG_REQUEST_RENDER          = 3;
    public static final int MSG_DECODING_TIME           = 4;

    public static final int MEDIA_PARAM_VIDEO_WIDTH     = 0x0001;
    public static final int MEDIA_PARAM_VIDEO_HEIGHT    = 0x0002;
    public static final int MEDIA_PARAM_VIDEO_DURATION  = 0x0003;

    private long mNativePlayerHandle = 0;
    private EventCallback eventCallback;
    public void addEventCallback(EventCallback eventCallback) {
        this.eventCallback = eventCallback;
    }
    public long getMediaParams(int paramType) {
        return nativeGetMediaParams(mNativePlayerHandle, paramType);
    }
    private void playerEventCallback(int msgType, float msgValue) {
        LogUtil.d(TAG,String.format(Locale.getDefault(), "onEventCallback: %d --- %f", msgType, msgValue));
        if (this.eventCallback != null) {
            this.eventCallback.onPlayerEvent(msgType, msgValue);
        }
    }

    public static String getFFmpegVersion() {
        return nativeGetFFmpegVersion();
    }
    private static native String nativeGetFFmpegVersion();
    private static native long nativeGetMediaParams(long playerHandle, int paramType);

    public void init(String url, int videoRenderType, Surface surface) {
        mNativePlayerHandle = nativeInit(url, FFMEDIA_PLAYER, videoRenderType, surface);
    }

    public void play() {
        nativePlay(mNativePlayerHandle);
    }

    private native long nativeInit(String url, int playerType, int renderType, Object surface);
    private native void nativePlay(long playerHandle);

    public interface EventCallback {
        void onPlayerEvent(int msgType, float msgValue);
    }
}
