package com.lyhao.xvideoplayer.ui;

import static com.lyhao.xvideoplayer.media.FFMediaPlayer.MEDIA_PARAM_VIDEO_DURATION;
import static com.lyhao.xvideoplayer.media.FFMediaPlayer.MEDIA_PARAM_VIDEO_HEIGHT;
import static com.lyhao.xvideoplayer.media.FFMediaPlayer.MEDIA_PARAM_VIDEO_WIDTH;
import static com.lyhao.xvideoplayer.media.FFMediaPlayer.MSG_DECODER_DONE;
import static com.lyhao.xvideoplayer.media.FFMediaPlayer.MSG_DECODER_READY;
import static com.lyhao.xvideoplayer.media.FFMediaPlayer.MSG_DECODING_TIME;
import static com.lyhao.xvideoplayer.media.FFMediaPlayer.VIDEO_RENDER_ANWINDOW;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.SeekBar;
import android.widget.Toast;

import com.lyhao.xvideoplayer.R;
import com.lyhao.xvideoplayer.media.FFMediaPlayer;
import com.lyhao.xvideoplayer.media.MySurfaceView;
import com.lyhao.xvideoplayer.util.LogUtil;

import java.util.Locale;

public class NativeMediaPlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback, FFMediaPlayer.EventCallback {
    public static final String TAG = "NativeMediaPlayerAct";
    private MySurfaceView mSurfaceView;
    private SeekBar seek_bar;
    private boolean mIsTouch;
    private FFMediaPlayer ffMediaPlayer;
    private static final int PERMISSION_REQUEST_CODE = 1;
    private String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/byteflow/oceans.mp4";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_media_player);
        mSurfaceView = findViewById(R.id.surface_view);
        mSurfaceView.getHolder().addCallback(this);

        seek_bar = findViewById(R.id.seek_bar);
        seek_bar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {
                mIsTouch = true;
            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                mIsTouch = false;
            }
        });
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        LogUtil.d(TAG, "========surfaceCreated========");
        ffMediaPlayer = new FFMediaPlayer();
        ffMediaPlayer.addEventCallback(this);
        ffMediaPlayer.init(mVideoPath, VIDEO_RENDER_ANWINDOW, surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        LogUtil.d(TAG, "========surfaceChanged========");
        ffMediaPlayer.play();
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        LogUtil.d(TAG, "========surfaceDestroyed========");
    }

    @Override
    protected void onResume() {
        super.onResume();
    }

    @Override
    public void onPlayerEvent(int msgType, float msgValue) {
        runOnUiThread(() -> {
            switch (msgType) {
                case MSG_DECODER_READY:{
                    onDecoderReady();
                    break;
                }
                case MSG_DECODER_DONE:{
                    break;
                }
                case MSG_DECODING_TIME: {
                    if (!mIsTouch) {
                        seek_bar.setProgress((int) msgValue);
                    }
                    break;
                }
            }
        });
    }

    private void onDecoderReady() {
        int width = (int)ffMediaPlayer.getMediaParams(MEDIA_PARAM_VIDEO_WIDTH);
        int height = (int)ffMediaPlayer.getMediaParams(MEDIA_PARAM_VIDEO_HEIGHT);
        LogUtil.d(TAG, String.format(Locale.getDefault(), "%d - %d", width, height));
        if (width * height != 0) {
            mSurfaceView.setAspectRatio(width, height);
        }

        int duration = (int) ffMediaPlayer.getMediaParams(MEDIA_PARAM_VIDEO_DURATION);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            seek_bar.setMin(0);
        }
        seek_bar.setMax(duration);
    }
}