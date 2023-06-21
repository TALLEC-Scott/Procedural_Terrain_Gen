using UnityEngine;

public class MapDisplay : MonoBehaviour
{ 
    public MeshFilter meshFilter;
    public Renderer textureRenderer;
   
    public MeshRenderer meshRenderer;

    public void DrawTexture(Texture2D texture)
    {
        textureRenderer.sharedMaterial.mainTexture = texture;
        textureRenderer.transform.localScale = new Vector3(texture.width, 1, texture.height);
    }
    
    public void DrawMesh(MeshGenerator.MeshData meshData, Texture2D texture)
    {
        meshFilter.sharedMesh = meshData.CreateMesh();
        meshRenderer.sharedMaterial.mainTexture = texture;
    }
    public void DrawMaterials(Material[] materials)
    {
        meshRenderer.sharedMaterials = materials;
    }
}
