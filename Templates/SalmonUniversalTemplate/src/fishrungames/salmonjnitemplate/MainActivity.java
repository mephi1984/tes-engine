package fishrungames.salmonjnitemplate;

import fishrungames.engine.FileWrapper;

//Deprecated
//import fishrungames.androidjnitemplate.R;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.Bundle;

import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.GestureDetector.SimpleOnGestureListener;
import android.view.MotionEvent;
/*
import android.content.res.Resources;
import android.content.res.XmlResourceParser;
import android.util.AttributeSet;
import android.util.Xml;
import android.inputmethodservice.Keyboard;
import android.inputmethodservice.KeyboardView;
import android.view.ViewGroup.LayoutParams;
import android.widget.LinearLayout;
*/

//Deprecated
//import java.lang.reflect.Field;


public class MainActivity extends Activity
{

	GLView mView;
	
	boolean IsScrolling = false;

	private GestureDetector gestureDetector;
	
	@Override
	protected void onCreate(Bundle icicle)
	{
		
		super.onCreate(icicle);

		gestureDetector = new GestureDetector(new MyGestureListener());

		FileWrapper.LoadSalmonEngineLibrary();

		FileWrapper.SetActivityInstance(this);
		FileWrapper.SetupEnviroment();

		/*
		 * Deprecated
		try
		{
			for (Field f : R.raw.class.getFields())
			{
				FileWrapper.AddToFileMap(f.getName(), f.getInt(null));
			}

		
		} catch (IllegalArgumentException e)
		{
			FileWrapper.ConsoleOut("IllegalArgumentException\n");
			onStop();
		} catch (IllegalAccessException e)
		{
			FileWrapper.ConsoleOut("IllegalAccessException\n");
			onStop();
		}*/
		
		String apkFilePath = null;
		ApplicationInfo appInfo = null;
		PackageManager packMgmr = this.getPackageManager();
		try {
		        appInfo = packMgmr.getApplicationInfo("fishrungames.salmonjnitemplate", 0);
		    } catch (NameNotFoundException e) {
		 
		 e.printStackTrace();
		throw new RuntimeException("Unable to locate assets, aborting...");
		    }
		apkFilePath = appInfo.sourceDir;
		
		FileWrapper.SetupApkFilePath(apkFilePath);
		
		mView = new GLView(getApplication());
		
		setContentView(mView);
		
	}

	@Override
	protected void onPause()
	{
		JniWrapper.Destroy();
		super.onPause();
		mView.onPause();
	}

	@Override
	protected void onResume()
	{
		//Don't write anything here!
		super.onResume();
		mView.onResume();
	}

	@Override
	protected void onStop()
	{
		//Don't write anything here!
		super.onStop();
	}
	
	public boolean onKeyDown(int keyCode, KeyEvent event)
	{
		int ascii_keycode = keyCode;
			
		if (keyCode == KeyEvent.KEYCODE_DEL)
		{
			ascii_keycode = 8; //Hack - getUnicodeChar does not recognize backspace
		}
		else
		{
			ascii_keycode = event.getUnicodeChar();
		}
		
        JniWrapper.OnKeyPress(ascii_keycode);
		return super.onKeyDown(keyCode, event);
	}

	public boolean onTouchEvent(MotionEvent event)
	{
		if (gestureDetector.onTouchEvent(event))
		{
			return true;
		}

		if (event.getAction() == MotionEvent.ACTION_UP)
		{
			float x = event.getX();
			float y = (float) mView.getHeight() - event.getY();

			if (IsScrolling)
			{
				IsScrolling = false;
			}

			JniWrapper.OnTapUp(x, y, event.getEventTime());

		}
		return true;
	}

	class MyGestureListener extends SimpleOnGestureListener
	{
		
		@Override
		public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
				float velocityY)
		{
			JniWrapper.OnFling(velocityX, velocityY, e2.getEventTime());
			return true;
		}

		public boolean onScroll(MotionEvent e1, MotionEvent e2,
				float distanceX, float distanceY)
		{
			
			JniWrapper.OnScroll(distanceX, distanceY, e2.getEventTime());
			IsScrolling = true;
			return true;
		}

		public boolean onDown(MotionEvent event)
		{

			float x = event.getX();
			float y = (float) mView.getHeight() - event.getY();
		
			JniWrapper.OnTapDown(x, y, event.getEventTime());
			
			return true;
		}

	}

}