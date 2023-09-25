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

    struct RectProperties
    {
        public static RectProperties Zero = new RectProperties(0, 0, 0, 0);
        public float left;
        public float bottom;
        public float right;
        public float top;
        public RectProperties(float left, float bottom, float right, float top)
        {
            this.left = left;
            this.bottom = bottom;
            this.right = right;
            this.top = top;
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

    struct Rect
    {
        public RectProperties offset;
        public RectProperties margin;
        public RectProperties padding;

        /// <summary>
        /// Default constructor with normalized offsets.
        /// </summary>
        /// <param name="left"></param>
        /// <param name="bottom"></param>
        /// <param name="right"></param>
        /// <param name="top"></param>
        public Rect(float left, float bottom, float right, float top)
        {
            offset.left = left;
            offset.bottom = bottom;
            offset.right = right;
            offset.top = top;

            margin = RectProperties.Zero;
            padding = RectProperties.Zero;
        }

        /// <summary>
        /// Generate Rect by using a normalized position and width and height pixel parameters.
        /// </summary>
        /// <param name="left">Normalized position.</param>
        /// <param name="bottom">Normalized position.</param>
        /// <param name="width">Width in pixels.</param>
        /// <param name="height">Height in pixels.</param>
        public static Rect RectWithPixelSize(float left, float bottom, float width, float height)
        {
            RectProperties rectOffset = RectProperties.Zero;
            rectOffset.left = left;
            rectOffset.bottom = bottom;

            Vector2 anchorLB = new Vector2(left, bottom);
            Vector2 anchorRT = anchorLB;
            Vector2 anchorRTinPixelSpace = UIUtility.NormalizedSpaceCoordToPixelCoord(anchorRT);
            anchorRTinPixelSpace.x += width;
            anchorRTinPixelSpace.y -= height;

            Vector2 anchorRTinNormalizedSpace = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorRTinPixelSpace);

            rectOffset.right = anchorRTinNormalizedSpace.x;
            rectOffset.top = anchorRTinNormalizedSpace.y;

            return new Rect(left, bottom, rectOffset.right, rectOffset.top);
        }

        /// <summary>
        /// Generate Rect by using a pixel position and width and height pixel parameters. x and y range from 0 to screen width and height. 0 is left side or top of the screen.
        /// </summary>
        /// <param name="x">Pixel position x</param>
        /// <param name="y">Pixel position y.</param>
        /// <param name="width">Width in pixels.</param>
        /// <param name="height">Height in pixels.</param>
        public static Rect RectWithPixelPositionAndSize(float x, float y, float width, float height)
        {
            RectProperties rectOffset = RectProperties.Zero;

            Vector2 anchorLBinPixelSpace = new Vector2(x, y);
            Vector2 anchorLBinNormalizedSpace = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorLBinPixelSpace);
            rectOffset.left = anchorLBinNormalizedSpace.x;
            rectOffset.bottom = anchorLBinNormalizedSpace.y;

            Vector2 anchorRTinPixelSpace = anchorLBinPixelSpace;
            anchorRTinPixelSpace.x += width;
            anchorRTinPixelSpace.y -= height;

            Vector2 anchorRTinNormalizedSpace = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorRTinPixelSpace);

            rectOffset.right = anchorRTinNormalizedSpace.x;
            rectOffset.top = anchorRTinNormalizedSpace.y;

            return new Rect(rectOffset.left, rectOffset.bottom, rectOffset.right, rectOffset.top);
        }

        /// <summary>
        /// 
        /// </summary>
        /// <returns>Normalized left bottom coords.</returns>
        public Vector2 GetAnchorLeftBottom() => new Vector2(offset.left, offset.bottom);
        /// <summary>
        /// 
        /// </summary>
        /// <returns>Normalized right top coords.</returns>
        public Vector2 GetAnchorRightTop() => new Vector2(offset.right, offset.top);

        /// <summary>
        /// 
        /// </summary>
        /// <returns>Normalized left bottom coords.</returns>
        public Vector2 GetAnchorLeftBottomPixelSpace() => UIUtility.NormalizedSpaceCoordToPixelCoord(GetAnchorLeftBottom());
        /// <summary>
        /// 
        /// </summary>
        /// <returns>Normalized right top coords.</returns>
        public Vector2 GetAnchorRightTopPixelSpace() => UIUtility.NormalizedSpaceCoordToPixelCoord(GetAnchorRightTop());

        /// <summary>
        /// </summary>
        /// <returns>Absolute normalized width of rect.</returns>
        public float GetWidthNormalized()
        {
            float val = offset.right - offset.left;
            return val < 0 ? -val : val;
        }

        /// <summary>
        /// </summary>
        /// <returns>Absolute normalized height of rect.</returns>
        public float GetHeightNormalized()
        {
            float val = offset.top - offset.bottom;
            return val < 0 ? -val : val;
        }

        /// <summary>
        /// </summary>
        /// <returns>Absolute width in pixels of rect.</returns>
        public float GetWidthPixels()
        {
            float val = GetAnchorLeftBottomPixelSpace().x - GetAnchorRightTopPixelSpace().x;
            return val < 0 ? -val : val;
        }

        /// <summary>
        /// </summary>
        /// <returns>Absolute height in pixels of rect.</returns>
        public float GetHeightPixels()
        {
            float val = GetAnchorLeftBottomPixelSpace().y - GetAnchorRightTopPixelSpace().y;
            return val < 0 ? -val : val;
        }

        /// <summary>
        /// Move rect by normalized vector.
        /// </summary>
        /// <param name="normalizedPos"></param>
        public void MoveByNormalizedPosition(Vector2 normalized)
        {
            Vector2 anchorLB = GetAnchorLeftBottom();
            Vector2 anchorRT = GetAnchorRightTop();
            anchorLB += normalized;
            anchorRT += normalized;
            offset.left = anchorLB.x;
            offset.bottom = anchorLB.y;
            offset.right = anchorRT.x;
            offset.top = anchorRT.y;
        }

        /// <summary>
        /// Move rect by x amount of pixels.
        /// </summary>
        /// <param name="pixels">Amount of pixels to move. Can be negative.</param>
        public void MoveByPixelAmount(Vector2 pixels)
        {
            Vector2 anchorLB = GetAnchorLeftBottom();
            Vector2 anchorRT = GetAnchorRightTop();

            Vector2 anchorLBinPixels = UIUtility.NormalizedSpaceCoordToPixelCoord(anchorLB);
            Vector2 anchorRTinPixels = UIUtility.NormalizedSpaceCoordToPixelCoord(anchorRT);

            anchorLBinPixels += pixels;
            anchorRTinPixels += pixels;

            Vector2 normalizedAnchorLB = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorLBinPixels);
            Vector2 normalizedAnchorRT = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorRTinPixels);

            SetPositionNormalized(normalizedAnchorLB, normalizedAnchorRT);
        }

        /// <summary>
        /// Set the position of the rect direclty. Uses LB (left bottom) of rect as anchor.
        /// </summary>
        /// <param name="pixels">Pixel coords from 0 - Width and 0 to Height where 0 is the left side or the top side of the screen.</param>
        public void SetPositionByPixelCoords(Vector2 pixels)
        {
            Vector2 anchorLB = GetAnchorLeftBottom();
            Vector2 anchorRT = GetAnchorRightTop();
            Vector2 offsetInPixels = UIUtility.NormalizedSpaceCoordToPixelCoord(anchorRT) - UIUtility.NormalizedSpaceCoordToPixelCoord(anchorLB);

            //Directly update LB anchor since we use it for positioning
            Vector2 anchorLBinPixels = pixels;
            //Apply offset to RT anchor to avoid weird scaling of the rect
            Vector2 anchorRTinPixels = anchorLBinPixels + offsetInPixels;

            Vector2 normalizedAnchorLB = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorLBinPixels);
            Vector2 normalizedAnchorRT = UIUtility.PixelSpaceCoordToNormalizedCoord(anchorRTinPixels);

            SetPositionNormalized(normalizedAnchorLB, normalizedAnchorRT);
        }

        /// <summary>
        /// Set rect position in normalized space by anchor points.
        /// </summary>
        /// <param name="anchorLB">Normalized vector.</param>
        /// <param name="anchorRT">Normalized vector.</param>
        public void SetPositionNormalized(Vector2 anchorLB, Vector2 anchorRT)
        {
            offset.left = anchorLB.x;
            offset.bottom = anchorLB.y;
            offset.right = anchorRT.x;
            offset.top = anchorRT.y;
        }

        /// <summary>
        /// Set rect position in normalized space.
        /// </summary>
        /// <param name="left">Normalized offset from left side.</param>
        /// <param name="bottom">Normalized offset from bottom side.</param>
        /// <param name="right">Normalized offset from right side.</param>
        /// <param name="top"><Normalized offset from top side.</param>
        public void SetPositionNormalized(float left, float bottom, float right, float top)
        {
            offset.left = left;
            offset.bottom = bottom;
            offset.right = right;
            offset.top = top;
        }

        public void ApplyMargin(OffsetType offsetType)
        {
            Vector2 anchorLBinPixels = GetAnchorLeftBottomPixelSpace();
            Vector2 anchorRTinPixels = GetAnchorRightTopPixelSpace();

            float left = anchorLBinPixels.x;
            float bottom = anchorLBinPixels.y;
            float right = anchorRTinPixels.x;
            float top = anchorRTinPixels.y;

            switch (offsetType)
            {
                case OffsetType.LEFT:
                    left += margin.left;
                    break;
                case OffsetType.BOTTOM:
                    bottom -= margin.bottom;
                    break;
                case OffsetType.RIGHT:
                    right -= margin.right;
                    break;
                case OffsetType.TOP:
                    top += margin.top;
                    break;
                case OffsetType.ALL:
                    left += margin.left;
                    bottom -= margin.bottom;
                    right -= margin.right;
                    top += margin.top;
                    break;
            }

            Vector2 normalizedAnchorLB = UIUtility.PixelSpaceCoordToNormalizedCoord(new Vector2(left, bottom));
            Vector2 normalizedAnchorRT = UIUtility.PixelSpaceCoordToNormalizedCoord(new Vector2(right, top));
            SetPositionNormalized(normalizedAnchorLB, normalizedAnchorRT);
        }

        /// <summary>
        /// TODO
        /// </summary>
        public void ApplyPadding(OffsetType offsetType)
        {
            throw new NotImplementedException();
        }

        public void SetMargin(RectProperties margin)
        {
            this.margin = margin;
            ApplyMargin(OffsetType.ALL);
        }

        public void SetMargin(float left, float bottom, float right, float top)
        {
            margin.left = left;
            margin.bottom = bottom;
            margin.right = right;
            margin.top = top;
            ApplyMargin(OffsetType.ALL);
        }

        /// <summary>
        /// </summary>
        /// <param name="left">Top offset in pixels.</param>
        public void SetMarginLeft(float left)
        {
            margin.left = left;
            ApplyMargin(OffsetType.LEFT);
        }

        /// <summary>
        /// </summary>
        /// <param name="bot">Top offset in pixels.</param>
        public void SetMarginBottom(float bot)
        {
            margin.bottom = bot;
            ApplyMargin(OffsetType.BOTTOM);
        }

        /// <summary>
        /// </summary>
        /// <param name="right">Top offset in pixels.</param>
        public void SetMarginRight(float right)
        {
            margin.right = right;
            ApplyMargin(OffsetType.RIGHT);
        }

        /// <summary>
        /// </summary>
        /// <param name="top">Top offset in pixels.</param>
        public void SetMarginTop(float top)
        {
            margin.top = top;
            ApplyMargin(OffsetType.TOP);
        }

        public void SetPadding(RectProperties padding)
        {
            this.padding = padding;
            ApplyPadding(OffsetType.ALL);
        }

        public void SetPadding(float left, float bottom, float right, float top)
        {
            padding.left = left;
            padding.bottom = bottom;
            padding.right = right;
            padding.top = top;
            ApplyPadding(OffsetType.ALL);
        }

        /// <summary>
        /// </summary>
        /// <param name="left">Left offset in pixels.</param>
        public void SetPaddingLeft(float left)
        {
            padding.left = left;
            ApplyPadding(OffsetType.LEFT);
        }

        /// <summary>
        /// </summary>
        /// <param name="bot">Bottom offset in pixels.</param>
        public void SetPaddingBottom(float bot)
        {
            padding.bottom = bot;
            ApplyPadding(OffsetType.BOTTOM);
        }

        /// <summary>
        /// </summary>
        /// <param name="right">Right offset in pixels.</param>
        public void SetPaddingRight(float right)
        {
            padding.right = right;
            ApplyPadding(OffsetType.RIGHT);
        }

        /// <summary>
        /// </summary>
        /// <param name="top">Top offset in pixels.</param>
        public void SetPaddingTop(float top)
        {
            padding.top = top;
            ApplyPadding(OffsetType.TOP);
        }

        /// <summary>
        /// Draws the rectangle. Expects screen space to range from -1 to 1.
        /// </summary>
        /// <param name="graphic"></param>
        public void Draw(PX.Graphic graphic)
        {

            float left = offset.left;
            float bottom = offset.bottom;
            float right = offset.right;
            float top = offset.top;

            graphic.RectangleDraw(left, bottom, right, top);
        }

        /// <summary>
        /// TODO
        /// </summary>
        public void DrawOffset(PX.Graphic graphic)
        {
            throw new NotImplementedException();
        }

        public override string ToString()
        {
            return ("BL: " + GetAnchorLeftBottom().ToString() + "\nRT: " + GetAnchorRightTop().ToString());
        }

    }

}