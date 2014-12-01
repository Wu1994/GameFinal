#include "ModelFileGenerator.h"

void writeBonesTree(FILE* fp, const std::vector<SModelBoneWrapper>& bones)
{
	for (u32 i = 0; i < bones.size(); i++)
	{
		fwrite(&bones[i].Bone, sizeof(SModelBone), 1, fp);
	}

	//fwrite(&bones[0], sizeof(SModelBone), bones.size(), fp);
}

void writeAnimations(FILE* fp, const std::vector<SBoneAnimationClip>& clips)
{
	for (u32 i = 0; i < clips.size(); i++)
	{
		const SBoneAnimationClip& clip = clips[i];
		/* ��ÿ��AnimationClip, ��д��Name */
		fwrite(clip.Name, ANIMATION_CLIP_NAME_MAX_LENGTH, 1, fp);

		/* ������16λ��д���Clip���еĹ������� */
		u16 boneCount = clip.BoneAnimations.size();

		fwrite(&boneCount, sizeof(u16), 1, fp);

		for (u32 j = 0; j < clip.BoneAnimations.size(); j++)
		{
			const SBoneAnimation& boneAnimation = clip.BoneAnimations[j];

			/* ��ÿ��BoneAnimation, ����д��16λ�Ĺ���ID����ID�͹������е�ID��Ӧ */
			u16 boneId = boneAnimation.BoneID;
			fwrite(&boneId, sizeof(u16), 1, fp);

			/* �ֱ�д��ƽ�ơ����š���ת֡������ */
			u16 translate_frame_count = boneAnimation.TranslationFrames.size();
			u16 scale_frame_count = boneAnimation.ScaleFrames.size();
			u16 rotate_frame_count = boneAnimation.RotationFrames.size();

			fwrite(&translate_frame_count, sizeof(u16), 1, fp);
			fwrite(&scale_frame_count, sizeof(u16), 1, fp);
			fwrite(&rotate_frame_count, sizeof(u16), 1, fp);

			/* ��ƽ�ơ����š���ת�ľ���framesд�� */
			fwrite(&boneAnimation.TranslationFrames[0], sizeof(STranslationAnimateFrame), translate_frame_count, fp);
			fwrite(&boneAnimation.ScaleFrames[0], sizeof(SScaleAnimateFrame), scale_frame_count, fp);
			fwrite(&boneAnimation.RotationFrames[0], sizeof(SRotationAnimateFrame), rotate_frame_count, fp);
		}
	}
}

