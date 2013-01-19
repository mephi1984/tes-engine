package fishrungames.salmonjnitemplate;


import java.util.Calendar;

import android.content.Context;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import fishrungames.engine.GLViewAncestor;

class GLView extends GLViewAncestor
{
	static long lastTimeStamp;
	static boolean gameIsInited = false;

	public GLView(Context context)
	{
		//Change this method? Don't forget to change method below!
		super(context);
		init(false, 0, 0);
	}

	public GLView(Context context, boolean translucent, int depth, int stencil)
	{
		//Change this method? Don't forget to change method above!
		super(context);
		init(translucent, depth, stencil);
	}

	public void init(boolean translucent, int depth, int stencil)
	{
		super.init(translucent, depth, stencil);
		setRenderer(new Renderer());
		Calendar c = Calendar.getInstance();
		lastTimeStamp = c.getTimeInMillis();
		gameIsInited = true;
	}

	private static class Renderer implements GLSurfaceView.Renderer
	{
		public void onDrawFrame(GL10 gl)
		{
			if (gameIsInited)
			{
				Calendar c = Calendar.getInstance();

				long currentTimeStamp = c.getTimeInMillis();
		
				JniWrapper.Update(currentTimeStamp - lastTimeStamp);
			
				lastTimeStamp = currentTimeStamp;
			}
		}

		public void onSurfaceChanged(GL10 gl, int width, int height)
		{
			

		    //JniWrapper.Destroy();
			
			JniWrapper.Init(width,height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config)
		{
			//Do nothing.
		}
	}
}