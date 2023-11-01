package com.lyhao.xvideoplayer.ui;

import static com.lyhao.xvideoplayer.media.FFMediaPlayer.VIDEO_RENDER_ANWINDOW;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Environment;
import android.view.Menu;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import com.lyhao.xvideoplayer.R;
import com.lyhao.xvideoplayer.media.FFMediaPlayer;
import com.lyhao.xvideoplayer.media.MySurfaceView;
import com.lyhao.xvideoplayer.util.LogUtil;

public class NativeMediaPlayerActivity extends AppCompatActivity implements SurfaceHolder.Callback {
    public static final String TAG = "NativeMediaPlayerAct";
    private MySurfaceView mSurfaceView;
    private FFMediaPlayer ffMediaPlayer;
    private static final int PERMISSION_REQUEST_CODE = 1;
    private String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/byteflow/one_piece.mp4";
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_media_player);
        mSurfaceView = findViewById(R.id.surface_view);
        mSurfaceView.getHolder().addCallback(this);
    }

    @Override
    public void surfaceCreated(@NonNull SurfaceHolder surfaceHolder) {
        LogUtil.d(TAG, "========surfaceCreated========");
        ffMediaPlayer = new FFMediaPlayer();
        ffMediaPlayer.init(mVideoPath, VIDEO_RENDER_ANWINDOW, surfaceHolder.getSurface());
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder surfaceHolder, int i, int i1, int i2) {
        LogUtil.d(TAG, "========surfaceChanged========");
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder surfaceHolder) {
        LogUtil.d(TAG, "========surfaceDestroyed========");
    }

    @Override
    protected void onResume() {
        super.onResume();
    }
}