namespace PXUltimaCSTest
{
    enum OffsetType { LEFT, BOTTOM, RIGHT, TOP, ALL };
    struct Vector2
    {
        public static Vector2 Zero = new Vector2(0, 0);
        public float x;
        public float y;
        public Vector2(int x, int y)
        {
            this.x = x;
            this.y = y;
        }
        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public static Vector2 operator -(Vector2 a, Vector2 b) => new Vector2(a.x - b.x, a.y - b.y);
        public static Vector2 operator +(Vector2 a, Vector2 b) => new Vector2(a.x + b.x, a.y + b.y);
        public override string ToString()
        {
            return ("x: " + x.ToString() + " y: " + y.ToString());
        }
    }

    struct Vector4
    {
        public static Vector4 Black = new Vector4(0, 0, 0, 1);
        public float x;
        public float y;
        public float z;
        public float w;
        public float r => x;
        public float g => y;
        public float b => z;
        public float a => w;
        public Vector4(int x, int y, int z, int w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
    }

    struct RectProperties
    {
        public static RectProperties Zero = new RectProperties(0, 0, 0, 0);
        public float Left;
        public float Bottom;
        public float Right;
        public float Top;
        public RectProperties(float left, float bottom, float right, float top)
        {
            Left = left;
            Bottom = bottom;
            Right = right;
            Top = top;
        }
    }

    static class UIUtility
    {
        /// <summary>
        /// Temporary placeholder for current screen size.
        /// </summary>
        /// <returns>960</returns>
        public static int GetWindowWidthPixels() => 960;
        /// <summary>
        /// Temporary placeholder for current screen size.
        /// </summary>
        /// <returns>560</returns>
        public static int GetWindowHeightPixels() => 560;

        /// <summary>
        /// Converts a normalized coord to pixel coord with range from 0 - Width and 0 - Height.
        /// </summary>
        /// <param name="normalized">Coord from -1 to 1</param>
        /// <returns>Pixel coords from 0 - Width and 0 to Height where 0 is the left side or the top side of the screen.</returns>
        public static Vector2 NormalizedSpaceCoordToPixelCoord(Vector2 normalized)
        {
            float pixelX = (normalized.x + 1) * 0.5f * GetWindowWidthPixels();
            float pixelY = (1 - normalized.y) * 0.5f * GetWindowHeightPixels();
            return new Vector2(pixelX, pixelY);
        }

        /// <summary>
        /// Converts a pixel coord to normalized coord with range from -1 to 1.
        /// </summary>
        /// <param name="pixelCoord"></param>
        /// <returns>Pixel coords from 0 - Width and 0 to Height where 0 is the left side or the top side of the screen.</returns>
        public static Vector2 PixelSpaceCoordToNormalizedCoord(Vector2 pixelCoord)
        {
            float normalizedX = (2 * pixelCoord.x / GetWindowWidthPixels()) - 1;
            float normalizedY = 1 - (2 * pixelCoord.y / GetWindowHeightPixels());
            return new Vector2(normalizedX, normalizedY);
        }

    }

}