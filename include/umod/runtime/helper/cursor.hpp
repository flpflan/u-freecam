#pragma once

namespace umod::unity_runtime::helper
{
    namespace CursorUtils
    {
        void disableCursor();
        void enableCursor();
        void toggleCursor();

        void backup();
        void resume();
    }
}
