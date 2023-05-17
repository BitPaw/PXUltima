using System;

namespace PX
{
    internal enum PXOpenGLVersion
    {

    }

    internal unsafe struct PXOpenGL
    {
        public void* PXOpenGLConext;

        public fixed byte Vendor[64];
        public fixed byte Renderer[64];
        public fixed byte VersionText[64];
        public fixed byte GLSLVersionText[64];
        public PXOpenGLVersion Version;

        public void* AttachedWindow;
    }

    public class OpenGL : IDisposable
    {
        private PXOpenGL _pxOpenGL = new PXOpenGL();

        public string Vendor 
        {
            get 
            {
                return string.Empty;
            }
        }
        public string Renderer
        {
            get
            {
                return string.Empty;
            }
        }
        public string VersionText
        {
            get
            {
                return string.Empty;
            }
        }
        public string GLSLVersionText
        {
            get
            {
                return string.Empty;
            }
        }
        //  public PXOpenGLVersion Version;

        public OpenGL()
        {

        }

        public void Dispose()
        {
          
        }
    }
}
