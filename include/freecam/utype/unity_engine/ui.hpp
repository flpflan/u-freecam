#pragma once

#include "core.hpp"
#include "utype/utype.hpp"

namespace UTYPE
{
    enum class RenderMode
    {
        ScreenSpaceOverlay,
        ScreenSpaceCamera,
        WorldSpace
    };
    class Canvas : public UTYPE::Behaviour
    {
        UNITY_CLASS_DECL("UnityEngine.UIModule.dll", Canvas)
        UNITY_METHOD(RenderMode, get_renderMode, (void))
        UNITY_METHOD(void, set_renderMode, (const RenderMode mode), mode)
        UNITY_METHOD(bool, get_overrideSorting, (void))
        UNITY_METHOD(void, set_overrideSorting, (const bool o), o)
        UNITY_METHOD(int, get_sortingOrder, (void))
        UNITY_METHOD(void, set_sortingOrder, (const int o), o)
    };

    class UIBehaviour : public UTYPE::MonoBehaviour
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", UIBehaviour)
    };
    class CanvasScaler : public UIBehaviour
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", CanvasScaler)
        enum class ScaleMode
        {
            ConstantPixelSize,
            ScaleWithScreenSize,
            ConstantPhysicalSize
        };
        UNITY_METHOD(ScaleMode, get_uiScaleMode, (void))
        UNITY_METHOD(void, set_uiScaleMode, (const ScaleMode o), o)
        UNITY_METHOD(UTYPE::Vector2, get_referenceResolution, (void))
        UNITY_METHOD(void, set_referenceResolution, (const UTYPE::Vector2 &o), o)
    };
    class BaseRaycaster : public UIBehaviour
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", BaseRaycaster)
    };
    class GraphicRaycaster : public BaseRaycaster
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", GraphicRaycaster)
    };
    class Graphic : public UIBehaviour
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", Graphic)
        UNITY_METHOD(UTYPE::Color, get_color, (void))
        UNITY_METHOD(void, set_color, (const UTYPE::Color &color), color)
    };
    class MaskableGraphic : public Graphic
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", MaskableGraphic)
    };
    class Image : public MaskableGraphic
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", Image)
        // UNITY_METHOD(UTYPE::Material *, get_material, (void))
        // UNITY_METHOD(void, set_material, (const UTYPE::Material &material), material)
    };
    class Selectable : public UIBehaviour
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", Selectable)
    };
    class Button : public Selectable
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", Button)
    };
    enum class TextAnchor
    {
        UpperLeft,
        UpperCenter,
        UpperRight,
        MiddleLeft,
        MiddleCenter,
        MiddleRight,
        LowerLeft,
        LowerCenter,
        LowerRight
    };
    class Text : public MaskableGraphic
    {
        UNITY_CLASS_DECL("UnityEngine.UI.dll", Text)
        UNITY_METHOD(UTYPE::String *, get_text, (void))
        UNITY_METHOD(void, set_text, (const UTYPE::String *text), text)
        UNITY_METHOD(TextAnchor, get_alignment, (void))
        UNITY_METHOD(void, set_alignment, (const TextAnchor &alignment), alignment)
    };
}
