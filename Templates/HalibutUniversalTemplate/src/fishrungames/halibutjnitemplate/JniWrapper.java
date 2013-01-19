package fishrungames.halibutjnitemplate;

public class JniWrapper
{
    static {
        System.loadLibrary("AndroidJniTemplate");
    }

   
    public static native void Init(int width, int height);
    public static native void Update(long dt);
    public static native void StopSounds();
    public static native void Destroy();
    public static native int IsInited();
    public static native void OnTapDown(float x, float y, long time);
    public static native void OnTapUp(float x, float y, long time);
    public static native void OnTapMove(float x, float y, long time);
    
    public static native void OnFling(float velocityX, float velocityY, long time);
    public static native void OnScroll(float distanceX, float distanceY, long time);
    
    public static native void OnKeyPress(int keyCode);
}