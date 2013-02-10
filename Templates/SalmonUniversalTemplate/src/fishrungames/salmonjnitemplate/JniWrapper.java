package fishrungames.salmonjnitemplate;

public class JniWrapper
{
    static {
        System.loadLibrary("SalmonJniTemplate");
    }

   
    public static native void Init(int width, int height);
    
}