package fishrungames.wallpapertemplate;

public class JniWrapper
{

	static
	{
		System.loadLibrary("SalmonWallpaper");
	}
	
	public static void LoadLibrary()
	{
		//To force loading libraries
	}

	public static native void SetRedBkgPref(boolean r);
	
	
	public static native void Init(int width, int height);
	
	public static native void SetOffset(float offsetX, float offsetY);
	
}
