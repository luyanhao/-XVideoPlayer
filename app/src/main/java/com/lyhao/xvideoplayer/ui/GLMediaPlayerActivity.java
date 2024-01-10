package com.lyhao.xvideoplayer.ui;

import static com.lyhao.xvideoplayer.media.FFMediaPlayer.VIDEO_GL_RENDER;

import androidx.appcompat.app.AppCompatActivity;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;

import com.lyhao.xvideoplayer.R;
import com.lyhao.xvideoplayer.media.FFMediaPlayer;
import com.lyhao.xvideoplayer.media.MyGLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class GLMediaPlayerActivity extends AppCompatActivity implements GLSurfaceView.Renderer, FFMediaPlayer.EventCallback {

    private MyGLSurfaceView glSurfaceView;
    private FFMediaPlayer ffMediaPlayer;
    private String mVideoPath = Environment.getExternalStorageDirectory().getAbsolutePath() + "/byteflow/one_piece.mp4";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_glmedia_player);
        glSurfaceView = findViewById(R.id.gl_surface_view);

        ffMediaPlayer = new FFMediaPlayer();
        ffMediaPlayer.addEventCallback(this);
        ffMediaPlayer.init(mVideoPath, FFMediaPlayer.VIDEO_RENDER_OPENGL, null);
    }

    @Override
    public void onSurfaceCreated(GL10 gl10, EGLConfig eglConfig) {
        ffMediaPlayer.nativeOnSurfaceCreated(VIDEO_GL_RENDER);
    }

    @Override
    public void onSurfaceChanged(GL10 gl10, int i, int i1) {

    }

    @Override
    public void onDrawFrame(GL10 gl10) {

    }

    @Override
    public void onPlayerEvent(int msgType, float msgValue) {

    }
}