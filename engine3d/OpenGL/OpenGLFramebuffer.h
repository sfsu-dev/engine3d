#pragma once
#include <Core/internal/Core.h>
#include <engine3d/Graphics/Framebuffer.h>

namespace engine3d{
    class OpenGLFramebuffer : public Framebuffer {
    public:
        OpenGLFramebuffer(const FramebufferMetaData& framebufferMetaData);
        ~OpenGLFramebuffer();
        
        static int GetMaxFramebufferSize();

    private:
        void ApplyCurrentBoundState() override;
        void ReleaseCurrentBoundState() override;
        int GetPixelData(uint32_t attachmentIdx, int x, int y) override;
        void ClearAttachmentData(uint32_t attachmentIdx, int data) override;
        uint32_t GetPixelAttachmentID(uint32_t index = 0) const override;

        const FramebufferMetaData& GetFramebufferData() const override;

        uint32_t HasColorAttachmentID() override;
        uint32_t HasDepthAttachmentID() override;

        // void ResizeFramebuffer(uint32_t w, uint32_t h) override;

    private:
        void Invalidate();

    private:
        uint32_t m_Id;
        uint32_t m_TextureId;

        uint32_t m_ColorAttachment, m_DepthAttachment;
        FramebufferMetaData m_FramebufferMetaData;
        // std::vector<TextureFormat> m_AttachmentsFormats;
        // uint32_t m_AttachmentIndex = 0;
        // std::vector<FramebufferTextureMetaData> m_ColorAttachments;
        // FramebufferTextureMetaData m_DepthAttachmentData = TextureFormat::None;
    };
};