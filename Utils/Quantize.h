#ifndef __QUANTIZE_H_
#define __QUANTIZE_H_

typedef struct _NODE {
	BOOL bIsLeaf;				// TRUE if node has no children
	UINT nPixelCount;			// Number of pixels represented by this leaf
	UINT nRedSum;				// Sum of red components
	UINT nGreenSum;			 // Sum of green components
	UINT nBlueSum;				// Sum of blue components
	struct _NODE* pChild[8];	// Pointers to child nodes
	struct _NODE* pNext;		// Pointer to next reducible node
} NODE;

class CQuantizer
{
protected:
	NODE* m_pTree;
	UINT m_nLeafCount;
	NODE* m_pReducibleNodes[9];
	UINT m_nMaxColors;
	UINT m_nColorBits;

public:
	CQuantizer (UINT nMaxColors, UINT nColorBits);
	virtual ~CQuantizer ();
	BOOL ProcessImage (BYTE *pData, int iWidth, int iHeight );
	UINT GetColorCount ();
	void GetColorTable (RGBQUAD* prgb);

protected:
	int GetLeftShiftCount (DWORD dwVal);
	int GetRightShiftCount (DWORD dwVal);
	void AddColor (NODE** ppNode, BYTE r, BYTE g, BYTE b, UINT nColorBits,
		UINT nLevel, UINT* pLeafCount, NODE** pReducibleNodes);
	NODE* CreateNode (UINT nLevel, UINT nColorBits, UINT* pLeafCount,
		NODE** pReducibleNodes);
	void ReduceTree (UINT nColorBits, UINT* pLeafCount,
		NODE** pReducibleNodes);
	void DeleteTree (NODE** ppNode);
	void GetPaletteColors (NODE* pTree, RGBQUAD* prgb, UINT* pIndex);
};

#endif